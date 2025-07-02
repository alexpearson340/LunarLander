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
    , mTextures { std::vector<std::unique_ptr<Texture>>(static_cast<size_t>(FlightStatTexture::TOTAL)) }
    , mRenderer { renderer }
    , mFont { font }
{
    create();
}

void HeadsUpDisplay::create()
{   
    // make a texture for each display field
    for (auto& texture : mTextures)
    {
        texture = std::make_unique<Texture>(mRenderer, mFont);
    }
}

void HeadsUpDisplay::update(const FlightStats stats)
{   
    updateDisplayField(formatFloat(stats.noseAngle), FlightStatTexture::NOSE_ANGLE);
    updateDisplayField(formatFloat(stats.xPos), FlightStatTexture::X_POS);
    updateDisplayField(formatFloat(stats.yPos), FlightStatTexture::Y_POS);
    updateDisplayField(formatFloat(stats.xVel), FlightStatTexture::X_VEL);
    updateDisplayField(formatFloat(stats.yVel), FlightStatTexture::Y_VEL);
    updateDisplayField(formatFloat(stats.thrustUnits), FlightStatTexture::THRUST_UNITS);
}


void HeadsUpDisplay::updateDisplayField(const std::string & text, FlightStatTexture index)
{
    mTextures.at(static_cast<size_t>(index)).get()->loadFromRenderedText(
        std::string(DISPLAY_NAMES.at(static_cast<size_t>(index))) + text, 
        mTextColour, 
        mBackgroundColour
    );
}

std::string HeadsUpDisplay::formatFloat(float value)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value;
    return oss.str();
}

void HeadsUpDisplay::render() const
{   
    int x { mScreenWidth - X_DISPLAY_OFFSET};
    int y { mScreenHeight - mScreenHeight + 1 };
    for (auto& texture : mTextures)
    {
        texture.get()->render(x, y);
        y += Y_DISPLAY_OFFSET;
    }
}