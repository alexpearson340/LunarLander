#ifndef LUNARLANDERENGINE_H
#define LUNARLANDERENGINE_H

#include "engine/BaseEngine.h"
#include "engine/Timer.h"
#include "lunar_lander/Constants.h"
#include "lunar_lander/Spaceship.h"
#include "lunar_lander/TerrainGenerator.h"
#include "lunar_lander/HeadsUpDisplay.h"

class LunarLanderEngine : public BaseEngine
{
public:
    LunarLanderEngine();

private:
    bool loadMedia() override;
    bool create() override;
    bool update() override;
    bool render() override;
    
    bool renderTerrain();
    bool generateTerrain();
    void buildTerrainRenderData();
    bool spawnPlayer();
    bool createHeadsUpDisplay();

    std::vector<std::vector<int>> mTerrain;     // for collision physics
    std::vector<SDL_Point> mTerrainPoints;      // for more efficient rendering of the terrain line
    std::vector<SDL_Point> mForegroundPoints;   // for more efficient rendering of foreground below the terrain line
    Spaceship mPlayer;
    TerrainGenerator mTerrainGenerator;
    HeadsUpDisplay mHeadsUpDisplay;
    Timer mHudUpdateTimer{500};  // Update HUD every 500ms
};

#endif // LUNARLANDERENGINE_H