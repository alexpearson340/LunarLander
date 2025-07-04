#include <iostream>

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
    
    // Update HUD at controlled interval
    if (mHudUpdateTimer.shouldUpdate()) {
        mHeadsUpDisplay.update(mPlayer.getFlightStats());
    }
    
    return true;
}

bool LunarLanderEngine::render()
{
    // render background
    SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(mRenderer.get());

    // render terrain
    mTextures.at("terrain").get()->render(0, 0);
    
    // render objects
    mPlayer.render();
    mHeadsUpDisplay.render();

    return true;
}

void LunarLanderEngine::generateTerrain()
{
    // TODO config this somewhere sensible
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
    
    createTerrainTexture();
}

void LunarLanderEngine::createTerrainTexture()
{
    // create an in memory Texture and set it as the rendering target
    createTargetTexture("terrain", static_cast<int>(mTerrain[0].size()), static_cast<int>(mTerrain.size()));
    mTextures.at("terrain").get()->setAsRenderingTarget();

    // determine which points are foreground, terrain, or background
    std::vector<SDL_Point> foregroundPoints {};     // these are opaque black
    std::vector<SDL_Point> terrainPoints {};        // these are opaque white
    for (size_t x = 0; x < mTerrain[0].size(); x++)
    {   
        bool reachedForeground = false;
        for (size_t y = 0; y < mTerrain.size(); y++)
        {   
            SDL_Point point {static_cast<int>(x), static_cast<int>(y)};
            if (mTerrain[y][x] == 1)
            {   
                if (!reachedForeground)
                {   
                    terrainPoints.push_back(point);
                    reachedForeground = true;
                }
                else
                {
                    foregroundPoints.push_back(point);
                }
            }
        }
    }
    // fill the target texture with a transparent base (so that the background will show through it)
    SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 0, 0);
    SDL_RenderClear(mRenderer.get());

    // set the foregound pixels to opaque black
    SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 0, 255);
    SDL_RenderDrawPoints(mRenderer.get(), foregroundPoints.data(), static_cast<int>(foregroundPoints.size()));
    
    // set the terrain horizon line to opaque white
    SDL_SetRenderDrawColor(mRenderer.get(), 255, 255, 255, 255);
    SDL_RenderDrawPoints(mRenderer.get(), terrainPoints.data(), static_cast<int>(terrainPoints.size()));

    // return the render target to the screen itself
    SDL_SetRenderTarget(mRenderer.get(), nullptr);
}

void LunarLanderEngine::spawnPlayer()
{
    // todo printf
    std::cout << "Spawning player" << std::endl;
    mPlayer = Spaceship(100, 100, mTextures.at(SPACESHIP_TEXTURE).get());
}

void LunarLanderEngine::createHeadsUpDisplay()
{
    // todo printf
    std::cout << "Creating heads up display" << std::endl;
    mHeadsUpDisplay = HeadsUpDisplay(mScreenHeight, mScreenWidth, mRenderer.get(), mFont.get());
    mHeadsUpDisplay.update(mPlayer.getFlightStats());
}