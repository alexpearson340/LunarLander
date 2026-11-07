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