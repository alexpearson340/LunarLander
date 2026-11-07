#include "engine/SoundEffect.h"

SoundEffect::SoundEffect(Mix_Chunk* soundEffect)
    : mSoundEffect { soundEffect }
{
}

SoundEffect::~SoundEffect()
{
    Mix_FreeChunk(mSoundEffect);
}

void SoundEffect::play(int nLoops, int maxTimeMs)
{
    if (!Mix_Playing(1))
    {
        Mix_PlayChannelTimed(1, mSoundEffect, nLoops, maxTimeMs);

    }
}

void SoundEffect::fadeOut(int fadeTimeMs)
{
    Mix_FadeOutChannel(1, fadeTimeMs);
}

void SoundEffect::stop()
{
    Mix_HaltChannel(1);
}

void SoundEffect::setVolume(int volume)
{
    // 128 is the maximum volume. This is the default channel volume
    Mix_Volume(1, volume);
}