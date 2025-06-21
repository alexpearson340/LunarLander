#ifndef LUNARLANDERENGINE_H
#define LUNARLANDERENGINE_H

#include "engine/BaseEngine.h"
#include "lunar_lander/Constants.h"
#include "lunar_lander/TerrainGenerator.h"

class LunarLanderEngine : public BaseEngine
{
public:
    LunarLanderEngine();

private:
    bool loadMedia() override;
    bool create() override;
    bool update() override;
    bool render() override;

    bool generateTerrain();

    std::vector<std::vector<int>> mTerrain;
    TerrainGenerator mTerrainGenerator;
};

#endif // LUNARLANDERENGINE_H