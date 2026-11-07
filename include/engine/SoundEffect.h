#ifndef SOUNDEFFECT_H
#define SOUNDEFFECT_H

#include <SDL_mixer.h>

class SoundEffect
{
public:
    SoundEffect(Mix_Chunk*);
    ~SoundEffect();
    void play(int nLoops = 0, int maxTimeMs = -1);

private:
    Mix_Chunk* mSoundEffect;
};

#endif
