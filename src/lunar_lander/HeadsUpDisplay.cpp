#include <vector>
#include "lunar_lander/HeadsUpDisplay.h"

HeadsUpDisplay::HeadsUpDisplay()
    : mScreenHeight { 0 }
    , mScreenWidth { 0 }
    , mTextures {}
    , mRenderer { nullptr }
    , mFont { nullptr }
{

}

HeadsUpDisplay::HeadsUpDisplay(const int screenHeight, const int screenWidth, SDL_Renderer * renderer, TTF_Font * font)
    : mScreenHeight { screenHeight }
    , mScreenWidth { screenWidth }
    , mTextures { std::vector<std::unique_ptr<Texture>>(static_cast<size_t>(FlightStatTextures::TOTAL)) }
    , mRenderer { renderer }
    , mFont { font }
{
    for (auto& texture : mTextures)
    {
        texture = std::make_unique<Texture>(mRenderer, mFont);
    }
}

void HeadsUpDisplay::update(const FlightStats stats)
{   
    // todo hard coded
    std::string text { "Nose Angle " + std::to_string(stats.noseAngle) };
    mTextures.at(static_cast<size_t>(FlightStatTextures::NOSE_ANGLE)).get()->loadFromRenderedText(text, mTextColour, mBackgroundColour);
}

void HeadsUpDisplay::render() const
{   
    // todo hard coded
    int x { mScreenWidth - 300};
    int y { mScreenHeight - mScreenHeight + 5 };
    mTextures.at(static_cast<size_t>(FlightStatTextures::NOSE_ANGLE)).get()->render(x, y);
}