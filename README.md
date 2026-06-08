# LunarLander

Lunar Lander clone in C++ and SDL2

<img src="assets/lunar-lander.png" width="800" height="auto" />

## Highlights

- **Layered C++17 engine architecture.** `engine_lib` (RAII window, renderer, texture/font wrappers, timers, 2D vector math) + `lunar_lander_lib` for game logic. Memory is managed via `std::unique_ptr` with custom SDL deleters.

- **Vector-based physics with jerk-driven thrust controls.** Position, velocity, acceleration, thrust and gravity are all fully simulated physics `Vector2D` quantities. Holding thrust adds a *unit of jerk* (the time-derivative of acceleration) per frame, giving the controls a feeling of inertia. Live telemetry is rendered to the top-right.

- **Render-to-texture with parallax background.** Both the terrain and the starfield are rendered **once** into world-sized hardware textures - then blitted as a single texture per frame - no per-pixel work in the game loop. This sped the game up by over 10×. The starfield is offset relative to the player to provide a scrolling parallax background effect.

- **Procedural generation from a custom 1D Perlin noise implementation.** Perlin noise is used to generate the cratered terrain foreground and the background starfield.

- **World/viewport camera system.** The world is 10× wider than the screen and the camera follows the player, clamping at world edges. Paired with the parallax starfield background, giving a sense of scale.

- **Headless-testable game logic.** GoogleTest suite covers vector math and collision physics — including parameterized tests sweeping ship orientations through 8 angles. Collision logic is decoupled from SDL via a mock `Texture`, so tests run without a renderer.