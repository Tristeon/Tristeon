#include "AudioClip.h"
#include "Utils/Console.h"

#include "Audio.h"
#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

namespace Tristeon
{
	AudioClip::AudioClip()
	{
		AUDIO_ASSERT(alGenBuffers(1, &_buffer));
	}

	AudioClip::~AudioClip()
	{
		AUDIO_ASSERT(alDeleteBuffers(1, &_buffer));
	}

	AudioClip::AudioClip(const String& path)
	{
		//Create an OpenAL buffer to store our data
		AUDIO_ASSERT(alGenBuffers(1, &_buffer));
		
		//Load wav file
		drwav wav;
		if (!drwav_init_file(&wav, path.c_str(), NULL)) 
		{
			Console::warning("Failed to load wav file: " + path);
			return;
		}

		int16_t* pSampleData = (int16_t*)malloc((size_t)wav.totalPCMFrameCount * wav.channels * sizeof(int16_t));
		drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, pSampleData);

		AUDIO_ASSERT(alBufferData(_buffer, AL_FORMAT_MONO16, pSampleData, (size_t)wav.totalPCMFrameCount * wav.channels * sizeof(int16_t), wav.sampleRate))

		drwav_uninit(&wav);
		delete[] pSampleData;
	}

	AudioClip::AudioClip(void* data, const unsigned& size, const unsigned& frequency)
	{
		//Create an OpenAL buffer to store our data
		AUDIO_ASSERT(alGetError());
		AUDIO_ASSERT(alGenBuffers(1, &_buffer));

		AUDIO_ASSERT(alBufferData(_buffer, AL_FORMAT_MONO8, data, size, frequency));
	}
}