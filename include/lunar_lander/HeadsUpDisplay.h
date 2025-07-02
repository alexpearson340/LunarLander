#ifndef HEADSUPDISPLAY_H
#define HEADSUPDISPLAY_H

#include "lunar_lander/FlightStats.h"
#include <engine/Texture.h>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

class HeadsUpDisplay
{
public:
    HeadsUpDisplay();
    HeadsUpDisplay(const int, const int, SDL_Renderer*, TTF_Font*);

    void create();
    void update(const FlightStats stats);
    void render() const;

private:
    int mScreenHeight;
    int mScreenWidth;

    std::vector<std::unique_ptr<Texture>> mTextures;

    enum class FlightStatTexture
    {
        NOSE_ANGLE = 0,
        X_POS,
        Y_POS,
        X_VEL,
        Y_VEL,
        THRUST_UNITS,
        TOTAL
    };

    // compile time computed display strings, and offset
    static constexpr std::array<std::string_view, static_cast<size_t>(FlightStatTexture::TOTAL)> DISPLAY_NAMES {
        "Angle: ",
        "X Pos: ",
        "Y Pos: ",
        "X Vel: ",
        "Y Vel: ",
        "Thrust: "
    };
    // the length of the longest display name, plus some chars for the dynamic data
    // multiplied by an estimate of the number of pixels used per char
    static constexpr int X_PIXELS_PER_CHAR { 12 };
    static constexpr int Y_PIXELS_PER_CHAR { 24 };
    static constexpr int X_DISPLAY_OFFSET { (DISPLAY_NAMES.at(0).size() + 5) * X_PIXELS_PER_CHAR };
    static constexpr int Y_DISPLAY_OFFSET { 1 * Y_PIXELS_PER_CHAR };

    void updateDisplayField(const std::string&, const FlightStatTexture);
    std::string formatFloat(float value);

    SDL_Renderer* mRenderer;
    TTF_Font* mFont;

    // todo this should be the same as the rest of the engine
    SDL_Color mBackgroundColour { 0, 0, 0, 255 };
    SDL_Color mTextColour { 255, 255, 255, 255 };
};

#endif // HEADSUPDISPLAY_H