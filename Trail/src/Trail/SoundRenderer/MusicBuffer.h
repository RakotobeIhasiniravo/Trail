#pragma once
#include <AL/al.h>
#include <sndfile.h>

namespace Trail {
	class MusicBuffer
	{
	public:
		MusicBuffer(const char* filename);
		~MusicBuffer();

		void Play();   // play once
		void Loop();   // loop automatically when finished
		void Stop();   // stop playback
		void UpdateBufferStream(); // must be called regularly (e.g. every frame)
		ALint getSource();

	private:
		void RestartStream(); // internal helper

		ALuint p_Source;
		static const int BUFFER_SAMPLES = 8192;
		static const int NUM_BUFFERS = 4;
		ALuint p_Buffers[NUM_BUFFERS];
		SNDFILE* p_SndFile;
		SF_INFO p_Sfinfo;
		short* p_Membuf;
		ALenum p_Format;

		bool m_Looping;
		bool m_Playing;
	};
}