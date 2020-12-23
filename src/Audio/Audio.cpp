#include "Audio.h"

#include "AudioClip.h"
#include "AL/al.h"
#include "AL/alc.h"
#include "AssetManagement/Resources.h"
#include "Utils/Console.h"

// ReSharper disable once CppUnusedIncludeDirective (Required on GNU GCC)
#include <cfloat>

#include <thread>
#include <chrono>

namespace Tristeon
{
	Audio::Audio()
	{
		//Set up the ALC device, this will normally use the OS's default audio device
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
		AUDIO_ASSERT(alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED));
	}

	Audio::~Audio()
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(_context);
		alcCloseDevice(_device);
	}

	Audio::Handle Audio::play(const String& path, const bool& looping, const float& volume)
	{
		auto* clip = Resources::assetLoad<AudioClip>(path);
		return play(clip, looping, volume);
	}

	Audio::Handle Audio::play(const String& path, const Vector& position, const float& minimumRange, const float& maximumRange,
		const bool& looping, const float& volume)
	{
		auto* clip = Resources::assetLoad<AudioClip>(path);
		return play(clip, position, minimumRange, maximumRange, looping, volume);
	}

	Audio::Handle Audio::play(AudioClip* clip, const bool& looping, const float& volume)
	{
		//Gen source and bind clip
		ALuint source;
		AUDIO_ASSERT(alGenSources(1, &source));
		AUDIO_ASSERT(alSourcei(source, AL_BUFFER, clip->_buffer));

		//Set looping & volume
		AUDIO_ASSERT(alSourcei(source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE));
		AUDIO_ASSERT(alSourcef(source, AL_GAIN, volume));

		//Disable range based parameters
		AUDIO_ASSERT(alSourcef(source, AL_REFERENCE_DISTANCE, 0.0f));
		AUDIO_ASSERT(alSourcef(source, AL_MAX_DISTANCE, FLT_MAX));
		AUDIO_ASSERT(alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE));

		//Play, add and return
		AUDIO_ASSERT(alSourcePlay(source));
		addSource(Handle{ source }, looping);
		return Handle{ source };
	}

	Audio::Handle Audio::play(AudioClip* clip, const Vector& position, const float& minimumRange,
		const float& maximumRange, const bool& looping, const float& volume)
	{
		//Gen source and bind clip
		ALuint source;
		AUDIO_ASSERT(alGenSources(1, &source));
		AUDIO_ASSERT(alSourcei(source, AL_BUFFER, clip->_buffer));

		//Set looping & volume
		AUDIO_ASSERT(alSourcei(source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE));
		AUDIO_ASSERT(alSourcef(source, AL_GAIN, volume));

		AUDIO_ASSERT(alSourcef(source, AL_MIN_GAIN, 0));
		AUDIO_ASSERT(alSourcef(source, AL_MAX_GAIN, 1));

		//Set range & position params
		AUDIO_ASSERT(alSourcef(source, AL_REFERENCE_DISTANCE, minimumRange));
		AUDIO_ASSERT(alSourcef(source, AL_MAX_DISTANCE, maximumRange));
		AUDIO_ASSERT(alSource3f(source, AL_POSITION, position.x, position.y, -(int)Project::Graphics::tileWidth()));
		AUDIO_ASSERT(alSourcei(source, AL_SOURCE_RELATIVE, AL_FALSE));

		//Play, add and return
		AUDIO_ASSERT(alSourcePlay(source));
		addSource(Handle{ source }, looping);
		return Handle{ source };
	}

	bool Audio::isPlaying(const Handle& handle)
	{
		ALint playing;
		AUDIO_ASSERT(alGetSourcei(handle.idx, AL_PLAYING, &playing));
		return playing;
	}

	void Audio::stop(Handle& handle)
	{
		AUDIO_ASSERT(alSourceStop(handle.idx));
		AUDIO_ASSERT(alDeleteSources(1, &handle.idx));
		instance()->_sources.remove(handle);
		handle.idx = 0;
	}

	void Audio::setPosition(const Handle& handle, const Vector& position)
	{
		AUDIO_ASSERT(alSource3f(handle.idx, AL_POSITION, position.x, position.y, -Project::Graphics::tileWidth()));
	}

	void Audio::addSource(const Handle& handle, const bool& looping)
	{
		instance()->_sources.add(handle);
		if (!looping)
		{
			std::thread(removeAfter, handle).detach();
		}
	}

	void Audio::removeAfter(Handle handle)
	{
		//Get playtime in ms
		ALint bufferID, bufferSize, frequency, bitsPerSample, channels;
		AUDIO_ASSERT(alGetSourcei(handle.idx, AL_BUFFER, &bufferID));
		AUDIO_ASSERT(alGetBufferi(bufferID, AL_SIZE, &bufferSize));
		AUDIO_ASSERT(alGetBufferi(bufferID, AL_FREQUENCY, &frequency));
		AUDIO_ASSERT(alGetBufferi(bufferID, AL_CHANNELS, &channels));
		AUDIO_ASSERT(alGetBufferi(bufferID, AL_BITS, &bitsPerSample));

		const auto seconds = static_cast<double>(bufferSize) / (frequency * channels * (bitsPerSample / 8)); //in seconds
		
		//Sleep thread until playtime is over
		std::this_thread::sleep_for(std::chrono::seconds(static_cast<int>(seconds + 1)));

		//Got removed before we got the chance
		if (!alIsSource(handle.idx))
			return;
		
		//Delete and cleanup
		AUDIO_ASSERT(alSourceStop(handle.idx));
		AUDIO_ASSERT(alDeleteSources(1, &handle.idx));
		instance()->_sources.remove(handle);
	}
}