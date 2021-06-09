#include "AudioSource.h"
#include "AL/al.h"
#include "AssetManagement/Resources.h"

namespace Tristeon
{
	AudioSource::AudioSource()
	{
		//Create OpenAL audio source
		AUDIO_ASSERT(alGenSources(1, &_handle));

		//Set defaults
		AUDIO_ASSERT(alSource3f(_handle, AL_POSITION, 0, 0, 0));
		AUDIO_ASSERT(alSource3f(_handle, AL_VELOCITY, 0, 0, 0));
		AUDIO_ASSERT(alSourcef(_handle, AL_MIN_GAIN, 0));
		AUDIO_ASSERT(alSourcef(_handle, AL_MAX_GAIN, 1));
		AUDIO_ASSERT(alSourcef(_handle, AL_REFERENCE_DISTANCE, 0));
		AUDIO_ASSERT(alSourcef(_handle, AL_MAX_DISTANCE, 256));
	}

	AudioSource::~AudioSource()
	{
		AUDIO_ASSERT(alSourceStop(_handle));
		AUDIO_ASSERT(alDeleteSources(1, &_handle));
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
		j["playOnInit"] = _playOnInit;

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

		_playOnInit = j.value("playOnInit", false);

		_clipPath = j.value("clipPath", "");
		setClip(Resources::load<AudioClip>(_clipPath));
	}

	void AudioSource::play()
	{
		AUDIO_ASSERT(alSourcePlay(_handle));
	}

	void AudioSource::pause()
	{
		AUDIO_ASSERT(alSourcePause(_handle));
	}
		
	void AudioSource::stop()
	{
		AUDIO_ASSERT(alSourceStop(_handle));
	}

	void AudioSource::init()
	{
		if (_playOnInit)
			play();
	}

	void AudioSource::lateUpdate()
	{
		//Update position
		AUDIO_ASSERT(alSource3f(_handle, AL_POSITION, actor()->position.x, actor()->position.y, -(int)Settings::Graphics::tileWidth())); //The z distance helps create smoother interactions since OpenAL isn't normally built for 2D audio
	}
}