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
    success = success && loadTexture("red.bmp");
    return success;
}

bool LunarLanderEngine::create()
{
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
    }
    return true;
}

bool LunarLanderEngine::render()
{   
    mInfoBar->render(0, mScreenHeight - BOTTOM_BAR_HEIGHT);
    return true;
}