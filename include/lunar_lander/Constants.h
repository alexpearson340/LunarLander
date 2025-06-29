#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// TODO
constexpr int SCREEN_WIDTH { 1080 };
constexpr int SCREEN_HEIGHT { 720 };

// Game variables
constexpr int ROTATION_SPEED { 30 };
constexpr float GRAVITY { 0.01f };

// Terrain generation
constexpr float TERRAIN_START_HEIGHT { 0.3f };       // fraction of SCREEN_HEIGHT where terrain gen starts
constexpr float TERRAIN_HEIGHT_VARIATION { 0.7f };   // fraction of SCREEN_HEIGHT as terrain
constexpr int PERLIN_OCTAVES { 4 };                  // Perlin Noise 1D parameter
constexpr double PERLIN_PERSISTENCE { 0.5 };         // Perlin Noise 1D parameter 
constexpr double PERLIN_FREQUENCY { 0.0025 };        // Perlin Noise 1D parameter

// Different textures for blocks
constexpr std::string_view SPACESHIP_TEXTURE { "spaceship.bmp" };

#endif // CONSTANTS_H
