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
    if (!Mix_Playing(SOUND_EFFECTS_CHANNEL))
    {
        Mix_PlayChannelTimed(SOUND_EFFECTS_CHANNEL, mSoundEffect, nLoops, maxTimeMs);
    }
}

void SoundEffect::fadeOut(int fadeTimeMs)
{
    Mix_FadeOutChannel(SOUND_EFFECTS_CHANNEL, fadeTimeMs);
}

void SoundEffect::stop()
{
    Mix_HaltChannel(SOUND_EFFECTS_CHANNEL);
}

void SoundEffect::setVolume(int volume)
{
    // 128 is the maximum volume. This is the default channel volume
    Mix_Volume(SOUND_EFFECTS_CHANNEL, volume);
}