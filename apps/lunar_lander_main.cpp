#define SDL_MAIN_HANDLED

#include "lunar_lander/LunarLanderEngine.h"

int main(int argc, char* args[])
{
    LunarLanderEngine game {};
    return game.run(argc, args);
}
