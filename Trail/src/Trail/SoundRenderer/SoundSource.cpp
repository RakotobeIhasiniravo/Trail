#include "trlpch.h"
#include "SoundSource.h"
#include <iostream>


namespace Trail {
	SoundSource::SoundSource(size_t poolSize)
	{
		p_Sources.resize(poolSize);
		alGenSources((ALsizei)poolSize, p_Sources.data());

		for (auto src : p_Sources)
		{
			alSourcef(src, AL_PITCH, 1.f);
			alSourcef(src, AL_GAIN, 1.f);
			alSource3f(src, AL_POSITION, 0.f, 0.f, 0.f);
			alSource3f(src, AL_VELOCITY, 0.f, 0.f, 0.f);
			alSourcei(src, AL_LOOPING, AL_FALSE);
			alSourcei(src, AL_BUFFER, 0);
		}
	}

	SoundSource::~SoundSource()
	{
		alDeleteSources((ALsizei)p_Sources.size(), p_Sources.data());
	}

	void SoundSource::Play(const ALuint buffer_to_play)
	{
		ALuint srcToUse = 0;
		bool foundFree = false;

		// Search for a source that is not playing
		for (auto src : p_Sources)
		{
			ALint state;
			alGetSourcei(src, AL_SOURCE_STATE, &state);
			if (state != AL_PLAYING)
			{
				srcToUse = src;
				foundFree = true;
				break;
			}
		}

		// If all sources are busy, just pick the first one (will cut it off)
		if (!foundFree)
			srcToUse = p_Sources[0];

		alSourceStop(srcToUse);
		alSourcei(srcToUse, AL_BUFFER, buffer_to_play);
		alSourcePlay(srcToUse);
	}

}


