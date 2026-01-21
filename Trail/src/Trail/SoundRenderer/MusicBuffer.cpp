#include "trlpch.h"
#include "MusicBuffer.h"
#include <cstddef>
#include <AL\alext.h>
#include <malloc.h>

namespace Trail {
	
	MusicBuffer::MusicBuffer(const char* filename)
		: p_SndFile(nullptr), p_Membuf(nullptr),
		m_Looping(false), m_Playing(false)
	{
		alGenSources(1, &p_Source);
		alGenBuffers(NUM_BUFFERS, p_Buffers);

		p_SndFile = sf_open(filename, SFM_READ, &p_Sfinfo);
		if (!p_SndFile)
			throw("could not open provided music file -- check path");

		if (p_Sfinfo.channels == 1)
			p_Format = AL_FORMAT_MONO16;
		else if (p_Sfinfo.channels == 2)
			p_Format = AL_FORMAT_STEREO16;
		else
			throw("Unsupported channel count from file");

		std::size_t frame_size = ((size_t)BUFFER_SAMPLES * (size_t)p_Sfinfo.channels) * sizeof(short);
		p_Membuf = static_cast<short*>(malloc(frame_size));
	}

	MusicBuffer::~MusicBuffer()
	{
		Stop();
		alDeleteSources(1, &p_Source);
		alDeleteBuffers(NUM_BUFFERS, p_Buffers);
		if (p_SndFile) sf_close(p_SndFile);
		free(p_Membuf);
	}

	void MusicBuffer::Play()
	{
		m_Looping = false;
		m_Playing = true;

		sf_seek(p_SndFile, 0, SEEK_SET);
		alSourceRewind(p_Source);
		alSourcei(p_Source, AL_BUFFER, 0);

		for (int i = 0; i < NUM_BUFFERS; i++)
		{
			sf_count_t slen = sf_readf_short(p_SndFile, p_Membuf, BUFFER_SAMPLES);
			if (slen < 1) break;

			slen *= p_Sfinfo.channels * (sf_count_t)sizeof(short);
			alBufferData(p_Buffers[i], p_Format, p_Membuf, (ALsizei)slen, p_Sfinfo.samplerate);
		}

		alSourceQueueBuffers(p_Source, NUM_BUFFERS, p_Buffers);
		alSourcePlay(p_Source);
	}

	void MusicBuffer::Loop()
	{
		m_Looping = true;
		m_Playing = true;
		Play();
	}

	void MusicBuffer::Stop()
	{
		m_Playing = false;
		alSourceStop(p_Source);
		alSourceRewind(p_Source);
	}

	void MusicBuffer::RestartStream()
	{
		sf_seek(p_SndFile, 0, SEEK_SET);
		for (int i = 0; i < NUM_BUFFERS; i++)
		{
			sf_count_t slen = sf_readf_short(p_SndFile, p_Membuf, BUFFER_SAMPLES);
			if (slen < 1) break;

			slen *= p_Sfinfo.channels * (sf_count_t)sizeof(short);
			alBufferData(p_Buffers[i], p_Format, p_Membuf, (ALsizei)slen, p_Sfinfo.samplerate);
		}

		alSourceQueueBuffers(p_Source, NUM_BUFFERS, p_Buffers);
		alSourcePlay(p_Source);
	}

	void MusicBuffer::UpdateBufferStream()
	{
		if (!m_Playing)
			return;

		ALint processed = 0;
		alGetSourcei(p_Source, AL_BUFFERS_PROCESSED, &processed);

		while (processed-- > 0)
		{
			ALuint bufid;
			alSourceUnqueueBuffers(p_Source, 1, &bufid);

			sf_count_t slen = sf_readf_short(p_SndFile, p_Membuf, BUFFER_SAMPLES);
			if (slen > 0)
			{
				slen *= p_Sfinfo.channels * (sf_count_t)sizeof(short);
				alBufferData(bufid, p_Format, p_Membuf, (ALsizei)slen, p_Sfinfo.samplerate);
				alSourceQueueBuffers(p_Source, 1, &bufid);
			}
			else if (m_Looping)
			{
				// if looping, restart the music
				sf_seek(p_SndFile, 0, SEEK_SET);
				sf_count_t slen2 = sf_readf_short(p_SndFile, p_Membuf, BUFFER_SAMPLES);
				slen2 *= p_Sfinfo.channels * (sf_count_t)sizeof(short);
				alBufferData(bufid, p_Format, p_Membuf, (ALsizei)slen2, p_Sfinfo.samplerate);
				alSourceQueueBuffers(p_Source, 1, &bufid);
			}
		}

		// restart playback if it stopped unexpectedly
		ALint state;
		alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING)
			alSourcePlay(p_Source);
	}

	ALint MusicBuffer::getSource()
	{
		return p_Source;
	}

}