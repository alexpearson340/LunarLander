#include "lunar_lander/TerrainGenerator.h"
#include <iostream>
#include <random>

TerrainGenerator::TerrainGenerator() :
    mNoise { std::random_device{}() }
{
};

void TerrainGenerator::generateTerrain(std::vector<std::vector<int>> & terrain, const TerrainGenerationConfig config)
{    
    mNoise = PerlinNoise1D(std::random_device{}());

    initializeTerrain(terrain, config);
    for (size_t x = 0; x < config.worldWidth; x++)
    {
        double noiseValue { mNoise.octaveNoise(x, config.octaves, config.persistence, config.scale) };
        int terrainHeight { config.startHeight + static_cast<int>(noiseValue * config.heightVariation) };
        for (size_t y = config.worldHeight - 1; y > config.worldHeight - static_cast<size_t>(terrainHeight); y--)
        {
            terrain[y][x] = 1;
        }
    }
    std::cout << "World size x=" << terrain[0].size() << " y=" << terrain.size() << '\n';
};

void TerrainGenerator::createTerrainTexture(SDL_Renderer* renderer, Texture* targetTexture, const std::vector<std::vector<int>>& terrain)
{
    // Set the passed texture as render target
    targetTexture->setAsRenderingTarget();

    // determine which points are foreground, terrain, or background
    std::vector<SDL_Point> foregroundPoints {};     // these are opaque black
    std::vector<SDL_Point> terrainPoints {};        // these are opaque white
    for (size_t x = 0; x < terrain[0].size(); x++)
    {   
        bool reachedForeground = false;
        for (size_t y = 0; y < terrain.size(); y++)
        {   
            SDL_Point point {static_cast<int>(x), static_cast<int>(y)};
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

void TerrainGenerator::initializeTerrain(std::vector<std::vector<int>> & terrain, const TerrainGenerationConfig config) const
{
    terrain = std::vector<std::vector<int>>{};
    terrain.resize(config.worldHeight);
    for (size_t y = 0; y < config.worldHeight; y++)
    {
        terrain[y].resize(config.worldWidth, 0);
    }
};