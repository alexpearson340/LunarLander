#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include "PerlinNoise1D.h"
#include "engine/Texture.h"
#include <SDL.h>
#include <vector>

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
    TerrainGenerator(const unsigned int seed);

    void generateTerrain(std::vector<std::vector<int>>&, const TerrainGenerationConfig &, const int);
    void createTerrainTexture(SDL_Renderer* renderer, Texture* targetTexture, const std::vector<std::vector<int>>& terrain) const;

private:
    void resizeTerrain(std::vector<std::vector<int>>&, const TerrainGenerationConfig &) const;
    int addLandingPad(std::vector<std::vector<int>>&, const TerrainGenerationConfig &, const int, const size_t);
    void fillTerrainUpToHeight(std::vector<std::vector<int>>&, const TerrainGenerationConfig &, const int, const size_t);
    bool shouldAddLandingPad();

    PerlinNoise1D mNoise;
    std::mt19937 mRandomNumberGenerator;
    double mLandingPadProbability;
};

#endif // TERRAINGENERATOR_H