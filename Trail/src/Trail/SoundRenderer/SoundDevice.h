#pragma once
#include <AL/alc.h>
namespace Trail {
	class SoundDevice
	{
	public:
		static SoundDevice* get();

	private:
		SoundDevice();
		~SoundDevice();

		ALCdevice* p_ALCDevice;
		ALCcontext* p_ALCContext;
	};
}


