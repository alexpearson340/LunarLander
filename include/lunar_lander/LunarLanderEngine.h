#ifndef LUNARLANDERENGINE_H
#define LUNARLANDERENGINE_H

#include "engine/BaseEngine.h"
#include "engine/Timer.h"
#include "lunar_lander/Constants.h"
#include "lunar_lander/Spaceship.h"
#include "lunar_lander/StarfieldGenerator.h"
#include "lunar_lander/TerrainGenerator.h"
#include "lunar_lander/HeadsUpDisplay.h"

enum class GameState {
    PLAYING,
    DEATH
};

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
    void spawnPlayer();
    void createHeadsUpDisplay();
    
    bool updatePlaying();
    bool updateDeath();

    std::vector<std::vector<int>> mTerrain;     // for collision physics
    Spaceship mPlayer;
    TerrainGenerator mTerrainGenerator;
    StarfieldGenerator mStarfieldGenerator;
    HeadsUpDisplay mHeadsUpDisplay;
    Timer mHudUpdateTimer{500};  // Update HUD every 500ms
    
    GameState mCurrentState = GameState::PLAYING;
};

#endif // LUNARLANDERENGINE_H