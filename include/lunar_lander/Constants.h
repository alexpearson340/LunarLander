#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// TODO
constexpr int SCREEN_WIDTH { 1350 };
constexpr int SCREEN_HEIGHT { 900 };
const std::string DISPLAY_TITLE { "Lunar Lander" };

// Game variables
constexpr float ROTATION_SPEED { 1.0f };
constexpr float ALIGNMENT_SPEED { 0.0f };
constexpr float GRAVITY { 0.005f };
constexpr float MAX_THRUST { 4 * GRAVITY };
constexpr float THRUST_UNIT { 0.1f * MAX_THRUST };

// Terrain generation
constexpr float TERRAIN_START_HEIGHT { 0.3f };       // fraction of SCREEN_HEIGHT where terrain gen starts
constexpr float TERRAIN_HEIGHT_VARIATION { 0.7f };   // fraction of SCREEN_HEIGHT as terrain
constexpr int PERLIN_OCTAVES { 4 };                  // Perlin Noise 1D parameter
constexpr double PERLIN_PERSISTENCE { 0.5 };         // Perlin Noise 1D parameter 
constexpr double PERLIN_FREQUENCY { 0.0025 };        // Perlin Noise 1D parameter

// Different textures for blocks
constexpr std::string_view SPACESHIP_TEXTURE { "spaceship.bmp" };

#endif // CONSTANTS_H
