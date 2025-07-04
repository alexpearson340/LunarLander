#ifndef LUNARLANDERENGINE_H
#define LUNARLANDERENGINE_H

#include "engine/BaseEngine.h"
#include "engine/Timer.h"
#include "lunar_lander/Constants.h"
#include "lunar_lander/Spaceship.h"
#include "lunar_lander/StarfieldGenerator.h"
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
    
    void generateTerrain();
    void generateBackground();
    void createTerrainTexture();
    void spawnPlayer();
    void createHeadsUpDisplay();

    std::vector<std::vector<int>> mTerrain;     // for collision physics
    Spaceship mPlayer;
    TerrainGenerator mTerrainGenerator;
    StarfieldGenerator mStarfieldGenerator;
    HeadsUpDisplay mHeadsUpDisplay;
    Timer mHudUpdateTimer{500};  // Update HUD every 500ms
};

#endif // LUNARLANDERENGINE_H