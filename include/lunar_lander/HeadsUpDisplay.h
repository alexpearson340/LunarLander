#ifndef HEADSUPDISPLAY_H
#define HEADSUPDISPLAY_H

#include <memory>
#include <vector>
#include "lunar_lander/FlightStats.h"
#include <engine/Texture.h>

class HeadsUpDisplay
{
public:
    HeadsUpDisplay();
    HeadsUpDisplay(const int, const int, SDL_Renderer*, TTF_Font*);

    void update(const FlightStats stats);
    void render() const;

private:
    int mScreenHeight;
    int mScreenWidth;

    std::vector<std::unique_ptr<Texture>> mTextures;
    
    enum class FlightStatTextures
    {
        NOSE_ANGLE = 0,
        X_POS,
        Y_POS,
        X_VEL,
        Y_VEL,
        X_ACCEL,
        Y_ACCEL,
        THRUST_UNITS,
        TOTAL
    };

    SDL_Renderer * mRenderer;
    TTF_Font * mFont;

    // todo this should be the same as the rest of the engine
    SDL_Color mBackgroundColour { 0, 0, 0, 255};
    SDL_Color mTextColour { 255, 255, 255, 255};
};

#endif  // HEADSUPDISPLAY_H