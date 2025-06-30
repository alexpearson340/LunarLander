#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// TODO
constexpr int SCREEN_WIDTH { 1350 };
constexpr int SCREEN_HEIGHT { 900 };

// Game variables
constexpr float ROTATION_SPEED { 1.0f };
constexpr float ALIGNMENT_SPEED { 0.5f };
constexpr float THRUST_UNIT { 0.005f };
constexpr float MAX_THRUST { 10 * THRUST_UNIT };
constexpr float GRAVITY { 5 * THRUST_UNIT };

// Terrain generation
constexpr float TERRAIN_START_HEIGHT { 0.3f };       // fraction of SCREEN_HEIGHT where terrain gen starts
constexpr float TERRAIN_HEIGHT_VARIATION { 0.7f };   // fraction of SCREEN_HEIGHT as terrain
constexpr int PERLIN_OCTAVES { 4 };                  // Perlin Noise 1D parameter
constexpr double PERLIN_PERSISTENCE { 0.5 };         // Perlin Noise 1D parameter 
constexpr double PERLIN_FREQUENCY { 0.0025 };        // Perlin Noise 1D parameter

// Different textures for blocks
constexpr std::string_view SPACESHIP_TEXTURE { "spaceship.bmp" };

#endif // CONSTANTS_H
