#ifndef LUNARLANDERENGINE_H
#define LUNARLANDERENGINE_H

#include "engine/BaseEngine.h"
#include "engine/Timer.h"
#include "lunar_lander/Constants.h"
#include "lunar_lander/HeadsUpDisplay.h"
#include "lunar_lander/Spaceship.h"
#include "lunar_lander/StarfieldGenerator.h"
#include "lunar_lander/TerrainGenerator.h"

enum class GameState
{
    PLAYING,
    DEATH
};

enum class Textures : size_t
{   
    SPACESHIP = 0,
    SPACESTATION = 1,
    TERRAIN = 2,
    STARFIELD = 3,
    TOTAL_TEXTURES = 4
};

enum class SoundEffects : size_t
{
    THRUSTER = 0,
    EXPLOSION = 1,
    TOTAL_SOUND_EFFECTS = 2
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

    std::vector<std::vector<int>> mTerrain; // for collision physics
    Spaceship mPlayer;
    TerrainGenerator mTerrainGenerator;
    StarfieldGenerator mStarfieldGenerator;
    HeadsUpDisplay mHeadsUpDisplay;
    Timer mHudUpdateTimer { 500 }; // Update HUD every 500ms

    GameState mCurrentState = GameState::PLAYING;
};

#endif // LUNARLANDERENGINE_H