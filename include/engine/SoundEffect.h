#ifndef SOUNDEFFECT_H
#define SOUNDEFFECT_H

#include <SDL_mixer.h>

constexpr int SOUND_EFFECTS_CHANNEL {1};

class SoundEffect
{
public:
    SoundEffect(Mix_Chunk*);
    ~SoundEffect();
    void play(int nLoops = 0, int maxTimeMs = -1);
    void fadeOut(int fadeTimeMs);
    void stop();
    void setVolume(int volume);

private:
    Mix_Chunk* mSoundEffect;
};

#endif
