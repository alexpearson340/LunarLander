#include <iostream>
#include <algorithm>

#include "lunar_lander/LunarLanderEngine.h"

LunarLanderEngine::LunarLanderEngine()
    : BaseEngine(SCREEN_HEIGHT, SCREEN_WIDTH, DISPLAY_TITLE) { };

bool LunarLanderEngine::loadMedia()
{
    bool success = true;
    mTextures.clear();

    // TODO - programmatically load textures in the assets dir
    success = success && loadTexture("spaceship.bmp");
    return success;
}

bool LunarLanderEngine::create()
{
    generateTerrain();
    generateBackground();
    spawnPlayer();
    createHeadsUpDisplay();
    return true;
}

bool LunarLanderEngine::update()
{
    // Handle events on queue
    while (SDL_PollEvent(&mEvent) != 0)
    {
        // User requests quit
        if (mEvent.type == SDL_QUIT)
        {
            mQuit = true;
        }
        // If a key was pressed
        else if (mEvent.type == SDL_KEYDOWN)
        {
            switch (mEvent.key.keysym.sym)
            {
            // regenerate the game - debugging convenience
            case SDLK_r:
                create();
                break;
            }
        }
    }

    // Handle continuous key input for rotation
    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    if (keyState[SDL_SCANCODE_LEFT])
    {
        mPlayer.rotate(-ROTATION_SPEED);
    }
    if (keyState[SDL_SCANCODE_RIGHT])
    {
        mPlayer.rotate(ROTATION_SPEED);
    }
    if (!keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RIGHT])
    {
        mPlayer.alignVertical(ALIGNMENT_SPEED);
    }

    // Handle holding down space to thrust, otherwise thrust decays
    if (keyState[SDL_SCANCODE_SPACE])
    {
        mPlayer.thrustIncrease();
    }
    else
    {
        mPlayer.thrustDecay();
    }

    // Handle physics
    mPlayer.updatePhysics();
    mPlayer.checkBoundaryCollision(WORLD_WIDTH, WORLD_HEIGHT);
    mPlayer.checkTerrainCollision(mTerrain);
    
    // Update HUD at controlled interval
    if (mHudUpdateTimer.shouldUpdate()) {
        mHeadsUpDisplay.update(mPlayer.getFlightStats());
    }
    
    return true;
}

bool LunarLanderEngine::render()
{   
    // Calculate desired camera position (world coordinates)
    float cameraWorldX = mPlayer.getPosX() - (mScreenWidth / 2);
    float cameraWorldY = mPlayer.getPosY() - (mScreenHeight / 2);
    
    // Clamp camera to world boundaries
    float clampedCameraX = std::clamp(cameraWorldX, 0.0f, static_cast<float>(WORLD_WIDTH - mScreenWidth));
    float clampedCameraY = std::clamp(cameraWorldY, 0.0f, static_cast<float>(WORLD_HEIGHT - mScreenHeight));
    
    // Convert to terrain screen position
    int terrainScreenX = -static_cast<int>(clampedCameraX);
    int terrainScreenY = -static_cast<int>(clampedCameraY);
    
    // Calculate player screen position relative to clamped camera
    int playerScreenX = static_cast<int>(mPlayer.getPosX() - clampedCameraX);
    int playerScreenY = static_cast<int>(mPlayer.getPosY() - clampedCameraY);

    // render background
    SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(mRenderer.get());
    
    // render world
    mTextures.at("starfield").get()->render(
        static_cast<int>(terrainScreenX * STARFIELD_PARALLAX_RATIO), 
        static_cast<int>(terrainScreenY * STARFIELD_PARALLAX_RATIO));
    mTextures.at("terrain").get()->render(terrainScreenX, terrainScreenY);
    
    // render objects
    mPlayer.render(playerScreenX, playerScreenY);
    mHeadsUpDisplay.render();

    return true;
}

void LunarLanderEngine::generateTerrain()
{
    // Initialise the terrain data structure
    TerrainGenerationConfig config {
        static_cast<size_t>(WORLD_WIDTH),
        static_cast<size_t>(WORLD_HEIGHT),
        static_cast<int>(WORLD_HEIGHT * TERRAIN_HEIGHT_VARIATION),
        static_cast<int>(WORLD_HEIGHT * TERRAIN_START_HEIGHT),
        PERLIN_OCTAVES,
        PERLIN_PERSISTENCE,
        PERLIN_FREQUENCY
    };
    // todo printf
    std::cout << "Generating terrain" << std::endl;
    mTerrainGenerator.generateTerrain(mTerrain, config);
    
    // Create terrain texture
    createTargetTexture("terrain", static_cast<int>(mTerrain[0].size()), static_cast<int>(mTerrain.size()));

    // Render terrain to the texture
    mTerrainGenerator.createTerrainTexture(mRenderer.get(), mTextures.at("terrain").get(), mTerrain);
}

void LunarLanderEngine::generateBackground()
{
    int starfieldWidth { static_cast<int>(SCREEN_WIDTH * (1 + STARFIELD_PARALLAX_RATIO)) };
    int starfieldHeight { static_cast<int>(SCREEN_HEIGHT * (1 + STARFIELD_PARALLAX_RATIO)) };

    std::cout << "Generating starfield background" << std::endl;
    mStarfieldGenerator.generateStarfield(starfieldWidth, starfieldHeight, 1500);
    
     // Create starfield texture
    createTargetTexture("starfield", starfieldWidth, starfieldHeight);
    
     // Render stars to the texture
     mStarfieldGenerator.createStarfieldTexture(mRenderer.get(), mTextures.at("starfield").get());
}

void LunarLanderEngine::spawnPlayer()
{
    std::cout << "Spawning player" << std::endl;
    mPlayer = Spaceship(100, 100, mTextures.at(SPACESHIP_TEXTURE).get(), GRAVITY, THRUST_UNIT, MAX_THRUST);
}

void LunarLanderEngine::createHeadsUpDisplay()
{
    std::cout << "Creating heads up display" << std::endl;
    mHeadsUpDisplay = HeadsUpDisplay(mScreenHeight, mScreenWidth, mRenderer.get(), mFont.get());
    mHeadsUpDisplay.update(mPlayer.getFlightStats());
}