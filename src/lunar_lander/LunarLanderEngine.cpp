#include <iostream>

#include "lunar_lander/LunarLanderEngine.h"

LunarLanderEngine::LunarLanderEngine()
    : BaseEngine(SCREEN_HEIGHT, SCREEN_WIDTH) { };

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
    spawnPlayer();
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
            // rotate the spaceship
            case SDLK_LEFT:
                mPlayer.rotate(-ROTATION_SPEED);
                break;
            case SDLK_RIGHT:
                mPlayer.rotate(ROTATION_SPEED);
                break;
            // regenerate the game  -   convenience
            case SDLK_r:
                generateTerrain();
                spawnPlayer();
                break;
            }
        }
    }

    // Handle physics
    mPlayer.updatePhysics();
    mPlayer.checkBoundaryCollision(mScreenWidth, mScreenHeight);

    return true;
}

bool LunarLanderEngine::render()
{
    // render terrain
    SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(mRenderer.get());
    SDL_SetRenderDrawColor(mRenderer.get(), 255, 255, 255, 255);
    for (size_t x = 0; x < mTerrain[0].size(); x++)
    {
        bool drawn { false };
        size_t y { 0 };
        while (!drawn)
        {
            if (mTerrain[y][x] == 1)
            {
                SDL_RenderDrawPoint(mRenderer.get(), static_cast<int>(x),
                    static_cast<int>(y));
                drawn = true;
            }
            y++;
        }
    }

    // render the player
    mPlayer.render();

    return true;
}

bool LunarLanderEngine::generateTerrain()
{
    // TODO config this somewhere sensible
    // Initialise the terrain data structure
    TerrainGenerationConfig config {
        static_cast<size_t>(mScreenWidth),
        static_cast<size_t>(mScreenHeight),
        static_cast<int>(mScreenHeight * TERRAIN_HEIGHT_VARIATION),
        static_cast<int>(mScreenHeight * TERRAIN_START_HEIGHT),
        PERLIN_OCTAVES,
        PERLIN_PERSISTENCE,
        PERLIN_FREQUENCY
    };
    std::cout << "Generating terrain" << std::endl;
    mTerrainGenerator.generateTerrain(mTerrain, config);
    return true;
}

bool LunarLanderEngine::spawnPlayer()
{
    std::cout << "Spawning player" << std::endl;
    mPlayer = Spaceship(100, 100, mTextures.at(SPACESHIP_TEXTURE).get());
    return true;
}