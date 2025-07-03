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

void TerrainGenerator::initializeTerrain(std::vector<std::vector<int>> & terrain, const TerrainGenerationConfig config) const
{
    terrain = std::vector<std::vector<int>>{};
    terrain.resize(config.worldHeight);
    for (size_t y = 0; y < config.worldHeight; y++)
    {
        terrain[y].resize(config.worldWidth, 0);
    }
};