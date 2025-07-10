#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// Screen/Viewport dimensions (what player sees)
constexpr int SCREEN_WIDTH { 1350 };
constexpr int SCREEN_HEIGHT { 900 };
const std::string DISPLAY_TITLE { "Lunar Lander" };

// World dimensions (actual game world size)
constexpr int WORLD_WIDTH { SCREEN_WIDTH * 10 };
constexpr int WORLD_HEIGHT { static_cast<int>(SCREEN_HEIGHT * 1.5) };
constexpr float STARFIELD_PARALLAX_RATIO { 1.0f / 50.0f };

// Game variables
constexpr float ROTATION_SPEED { 1.0f };
constexpr float ALIGNMENT_SPEED { 0.0f };
constexpr float GRAVITY { 0.01f };
constexpr float MAX_THRUST { 4 * GRAVITY };
constexpr float THRUST_UNIT { 0.1f * MAX_THRUST };

// Terrain generation
constexpr float TERRAIN_START_HEIGHT { 0.35f };       // fraction of WORLD_HEIGHT where terrain gen starts
constexpr float TERRAIN_HEIGHT_VARIATION { 0.35f };   // fraction of WORLD_HEIGHT as terrain
constexpr int PERLIN_OCTAVES { 4 };                  // Perlin Noise 1D parameter
constexpr double PERLIN_PERSISTENCE { 0.5 };         // Perlin Noise 1D parameter 
constexpr double PERLIN_FREQUENCY { 0.0025 };        // Perlin Noise 1D parameter

// Landing pads
constexpr unsigned int MIN_LANDING_PAD_WIDTH { 50 };
constexpr unsigned int MAX_LANDING_PAD_WIDTH { 100 };

// Different textures for blocks
constexpr std::string_view SPACESHIP_TEXTURE { "spaceship.bmp" };
constexpr std::string_view SPACESTATION_TEXTURE { "spacestation.bmp" };

#endif // CONSTANTS_H
