#include "Audio.h"
#include "AL/al.h"
#include "AL/alc.h"
#include "Utils/Console.h"

namespace Tristeon
{
	Audio::Audio()
	{
		//Set up the ALC device, this will normally use the OS's default audio device
		alGetError();
		_device = alcOpenDevice(nullptr);
		if (!_device)
			Console::error("Failed to open OpenAL device, audio will not function!");
		else
			Console::write("Initialized: OpenAL Audio device");

		//Create an audio context using the device
		alcGetError(_device);
		_context = alcCreateContext(_device, nullptr);
		ALCenum err = alcGetError(_device);
		Console::assertLog(err == ALC_NO_ERROR, "Failed to create ALC context: " + std::to_string(err) + ". audio will not function!");

		if (alcMakeContextCurrent(_context))
			Console::write("Initialized: Set ALC context to current");
		else
			Console::error("Failed to make ALC context current: " + std::to_string(alcGetError(_device)));

		//Set the distance model to linear distance because after testing it seems to be the most reliable solution for 2D audio
		alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
	}

	Audio::~Audio()
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(_context);
		alcCloseDevice(_device);
	}
}