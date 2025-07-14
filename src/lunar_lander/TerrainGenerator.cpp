#include "lunar_lander/Constants.h"
#include "lunar_lander/TerrainGenerator.h"
#include <iostream>
#include <random>

TerrainGenerator::TerrainGenerator(const unsigned int seed)
    : mNoise { seed }
    , mRandomNumberGenerator { seed } {
    };

void TerrainGenerator::generateTerrain(std::vector<std::vector<int>>& terrain, const TerrainGenerationConfig & config, const int averageNumberOfLandingPads)
{
    // Calculate landing pad probability once at the start
    mLandingPadProbability = static_cast<double>(averageNumberOfLandingPads) / static_cast<double>(config.worldWidth);
    resizeTerrain(terrain, config);

    size_t noiseX = 0;      // Continuous noise coordinate
    size_t terrainX = 0;    // Current terrain position
    int terrainHeight = config.startHeight; // Initialize with base height
    
    while (terrainX < config.worldWidth) {
        
        // randomly add a landing pad
        if (shouldAddLandingPad() && terrainX + MAX_LANDING_PAD_WIDTH < config.worldWidth) {
            size_t padWidth = static_cast<size_t>(addLandingPad(terrain, config, terrainHeight, terrainX));
            terrainX += padWidth;  // Skip ahead by pad width
        
        // otherwise keep generating noise based terrain
        } else {
            double noiseValue = mNoise.octaveNoise(noiseX, config.octaves, config.persistence, config.scale);
            terrainHeight = config.startHeight + static_cast<int>(noiseValue * config.heightVariation);
            fillTerrainUpToHeight(terrain, config, terrainHeight, terrainX);
            terrainX++;
            noiseX++;  // Only increment noise coordinate when we use a noise value - for continuous height on either side of a pad
        }
    }
    
    std::cout << "World size x=" << terrain[0].size() << " y=" << terrain.size() << '\n';
};

void TerrainGenerator::resizeTerrain(std::vector<std::vector<int>>& terrain, const TerrainGenerationConfig & config) const
{
    terrain = std::vector<std::vector<int>> {};
    terrain.resize(config.worldHeight);
    for (size_t y = 0; y < config.worldHeight; y++)
    {
        terrain[y].resize(config.worldWidth, TERRAIN_VACUUM);
    }
};

int TerrainGenerator::addLandingPad(std::vector<std::vector<int>>& terrain, const TerrainGenerationConfig & config, const int height, const size_t xPos)
{
    std::uniform_int_distribution<int> dist(MIN_LANDING_PAD_WIDTH, MAX_LANDING_PAD_WIDTH);
    size_t padLength = static_cast<size_t>(dist(mRandomNumberGenerator));
    
    for (size_t i = 0; i != padLength; i++)
    {
        fillTerrainUpToHeight(terrain, config, height, xPos + i);
    };
    return static_cast<int>(padLength);
};

void TerrainGenerator::fillTerrainUpToHeight(std::vector<std::vector<int>>& terrain, const TerrainGenerationConfig & config, const int height, const size_t xPos)
{
    for (size_t y = config.worldHeight - 1; y > config.worldHeight - static_cast<size_t>(height); y--)
    {
        terrain[y][xPos] = TERRAIN_ROCK;
    }
};

bool TerrainGenerator::shouldAddLandingPad()
{
    // Generate random number between 0.0 and 1.0
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double randomValue = dist(mRandomNumberGenerator);
    
    // Return true if random value is less than probability
    return randomValue < mLandingPadProbability;
};

void TerrainGenerator::createTerrainTexture(SDL_Renderer* renderer, Texture* targetTexture, const std::vector<std::vector<int>>& terrain) const
{
    // Set the passed texture as render target
    targetTexture->setAsRenderingTarget();

    // determine which points are foreground, terrain, or background
    std::vector<SDL_Point> foregroundPoints {}; // these are opaque black
    std::vector<SDL_Point> terrainPoints {}; // these are opaque white
    for (size_t x = 0; x < terrain[0].size(); x++)
    {
        bool reachedForeground = false;
        for (size_t y = 0; y < terrain.size(); y++)
        {
            SDL_Point point { static_cast<int>(x), static_cast<int>(y) };
            if (terrain[y][x] == 1)
            {
                if (!reachedForeground)
                {
                    terrainPoints.push_back(point);
                    reachedForeground = true;
                }
                else
                {
                    foregroundPoints.push_back(point);
                }
            }
        }
    }

    // fill the target texture with a transparent base (so that the background will show through it)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // set the foregound pixels to opaque black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawPoints(renderer, foregroundPoints.data(), static_cast<int>(foregroundPoints.size()));

    // set the terrain horizon line to opaque white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoints(renderer, terrainPoints.data(), static_cast<int>(terrainPoints.size()));

    // return the render target to the screen itself
    SDL_SetRenderTarget(renderer, nullptr);
}