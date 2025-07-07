#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <vector>
#include <SDL.h>
#include "PerlinNoise1D.h"
#include "engine/Texture.h"

struct TerrainGenerationConfig
{
    size_t worldWidth;
    size_t worldHeight;
    int heightVariation;
    int startHeight;
    int octaves;
    double persistence;
    double scale;
};

class TerrainGenerator 
{
public:
    TerrainGenerator();

    void generateTerrain(std::vector<std::vector<int>> &, TerrainGenerationConfig const);
    void createTerrainTexture(SDL_Renderer* renderer, Texture* targetTexture, const std::vector<std::vector<int>>& terrain);
    void initializeTerrain(std::vector<std::vector<int>> &, TerrainGenerationConfig const) const;

private:
    PerlinNoise1D mNoise;
};

#endif // TERRAINGENERATOR_H