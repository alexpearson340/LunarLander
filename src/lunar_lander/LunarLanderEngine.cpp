#include <iostream>

#include "lunar_lander/LunarLanderEngine.h"

LunarLanderEngine::LunarLanderEngine()
    : BaseEngine(SCREEN_HEIGHT, SCREEN_WIDTH)
{
};

bool LunarLanderEngine::loadMedia()
{
    bool success = true;
    mTextures.clear();

    // TODO - programmatically load textures in the assets dir
    success = success && loadTexture("background.bmp");
    return success;
}

bool LunarLanderEngine::create()
{   
    generateTerrain();
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
            // Adjust the velocity
            switch (mEvent.key.keysym.sym)
            {
            case SDLK_r:
                generateTerrain();
                break;
            }
        }
    }
    return true;
}

bool LunarLanderEngine::render()
{   
    SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(mRenderer.get());
    SDL_SetRenderDrawColor(mRenderer.get(), 255, 255, 255, 255);
    for (size_t x = 0; x < mTerrain[0].size(); x++)
    {
        bool drawn { false };
        size_t y { 0 };
        while ( !drawn )
        {
            if (mTerrain[y][x] == 1)
            {
                SDL_RenderDrawPoint(mRenderer.get(), static_cast<int>(x), static_cast<int>(y));
                drawn = true;
            }
            y++;
        }
    }
    return true;
}

bool LunarLanderEngine::generateTerrain()
{
    // TODO config this somewhere sensible
    // Initialise the terrain data structure
    TerrainGenerationConfig config {
        static_cast<size_t>(mScreenWidth),          // worldWidth
        static_cast<size_t>(mScreenHeight),         // worldHeight
        static_cast<int>(mScreenHeight * 0.7),      // heightVariation
        static_cast<int>(mScreenHeight * 0.5),      // startHeight
        4,                                          // octaves
        0.5,                                        // persistence
        0.0025                                      // frequency
    };
    std::cout << "Generating terrain" << std::endl;
    mTerrainGenerator.generateTerrain(mTerrain, config);
    return true;
}