#include "AudioSource.h"
#include "AL/al.h"
#include "AssetManagement/Resources.h"

namespace Tristeon
{
	AudioSource::AudioSource()
	{
		//Create OpenAL audio source
		alGetError();
		alGenSources(1, &_handle);
		Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to create audio source");

		//Set defaults
		alSource3f(_handle, AL_POSITION, 0, 0, 0);
		Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to set audio source position to a default value", AssertSeverity::Warning);
		alSource3f(_handle, AL_VELOCITY, 0, 0, 0);
		Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to set audio source velocity to a default value", AssertSeverity::Warning);
		alSourcef(_handle, AL_MIN_GAIN, 0);
		Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to set audio source minimum volume to a default value", AssertSeverity::Warning);
		alSourcef(_handle, AL_MAX_GAIN, 1);
		Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to set audio source maximum volume to a default value", AssertSeverity::Warning);
		alSourcef(_handle, AL_REFERENCE_DISTANCE, 0);
		Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to set audio source minimum range to a default value", AssertSeverity::Warning);
		alSourcef(_handle, AL_MAX_DISTANCE, 256);
		Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to set audio source maximum range to a default value", AssertSeverity::Warning);
	}

	AudioSource::~AudioSource()
	{
		alGetError();
		alSourceStop(_handle);
		Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to stop OpenAL source from playing prior to destruction", AssertSeverity::Warning);

		alDeleteSources(1, &_handle);
		Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to delete OpenAL audio source", AssertSeverity::Warning);
	}

	json AudioSource::serialize()
	{
		json j = Behaviour::serialize();
		j["typeID"] = Type<AudioSource>::fullName();

		j["pitch"] = _pitch;
		j["volume"] = _volume;

		j["minimumRange"] = _minimumRange;
		j["maximumRange"] = _maximumRange;
		j["minimumVolume"] = _minimumVolume;

		j["looping"] = _looping;
		j["playOnStart"] = _playOnStart;

		j["clipPath"] = _clipPath;
		return j;
	}

	void AudioSource::deserialize(json j)
	{
		Behaviour::deserialize(j);

		setPitch(j.value("pitch", 1.0f));
		setVolume(j.value("volume", 1.0f));

		setMinimumRange(j.value("minimumRange", 0.0f));
		setMaximumRange(j.value("maximumRange", 1028.0f));
		setMinimumVolume(j.value("minimumVolume", 0.0f));

		setLooping(j.value("looping", false));

		_playOnStart = j.value("playOnStart", false);

		_clipPath = j.value("clipPath", "");
		setClip(Resources::assetLoad<AudioClip>(_clipPath));
	}

	void AudioSource::play()
	{
		alGetError();
		alSourcePlay(_handle);
		Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to play audio source", AssertSeverity::Warning);
	}

	void AudioSource::stop()
	{
		alGetError();
		alSourceStop(_handle);
		Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to stop audio source", AssertSeverity::Warning);
	}

	void AudioSource::start()
	{
		if (_playOnStart)
			play();
	}

	void AudioSource::lateUpdate()
	{
		//Update position
		alGetError();
		alSource3f(_handle, AL_POSITION, actor()->position.x, actor()->position.y, -(int)Project::Graphics::tileWidth()); //The z distance helps create smoother interactions since OpenAL isn't normally built for 2D audio
		Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to set audio source position, audio distance function may behave incorrectly", AssertSeverity::Warning);
	}
}