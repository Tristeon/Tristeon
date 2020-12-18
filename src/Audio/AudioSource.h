#pragma once
#include "Scenes/Actors/Behaviour.h"
#include <Serialization/TypeRegister.h>
#include <Audio/AudioClip.h>
#include <Callbacks/ILateUpdate.h>
#include <Math/Math.h>
#include "AL/al.h"

namespace Tristeon
{
	/**
	 * AudioSource represents the physical source of a sound effect or song in-game.
	 * Audio in Tristeon uses linear attenuation to determine its volume/falloff over distance from the AudioListener. The attenuation behaviour can be tweaked through the AudioSource's fields.
	 */
	class AudioSource : public Behaviour, public IStart, public ILateUpdate
	{
	public:
		AudioSource();
		virtual ~AudioSource();
		
		[[nodiscard]] json serialize() override;
		void deserialize(json j) override;

		/**
		 * Start playing the attached clip.
		 * If no clip is attached, nothing happens.
		 */
		void play();
		/**
		 * Stop playing the attached clip.
		 * If no clip is attached, or the source isn't playing, nothing happens.
		 */
		void stop();

		/**
		 * The pitch of a sound is how high or low the sound is. A high sound has a high pitch and a low sound has a low pitch.
		 * This value is clamped between 0.5 and 2.0
		 */
		[[nodiscard]] float pitch() const { return _pitch; }
		/**
		 * Set the pitch.
		 * The pitch of a sound is how high or low the sound is. A high sound has a high pitch and a low sound has a low pitch.
		 * This value is clamped between 0.5 and 2.0
		 */
		void setPitch(const float& p)
		{
			_pitch = Math::clamp(p, 0.5f, 2.0f);
			alGetError();
			alSourcef(_handle, AL_PITCH, _pitch);
			Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to set audio source pitch. The change is likely not applied", AssertSeverity::Warning);
		}

		/**
		 * The volume/gain or loudness of the audio source.
		 *
		 * A value of 1.0 means unadjusted. Each division by 2 equals a reduction of about 6dB. Each multiplication by 2 equals an amplification of about 6dB.
		 * A value of 0.0 means the sound is entirely silent.
		 */
		[[nodiscard]] float volume() const { return _volume; }
		/**
		 * Set the volume/gain or loudness of the audio source.
		 *
		 * A value of 1.0 means unadjusted. Each division by 2 equals a reduction of about 6dB. Each multiplication by 2 equals an amplification of about 6dB.
		 * A value of 0.0 means the sound is entirely silent.
		 */
		void setVolume(const float& v)
		{
			_volume = v;
			alGetError();
			alSourcef(_handle, AL_GAIN, _volume);
			Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to set audio source volume. The change is likely not applied", AssertSeverity::Warning);
		}

		/**
		 * The minimum range is the distance from the center of the audio source at which the sound is still hearable at full volume.
		 */
		[[nodiscard]] float minimumRange() const { return _minimumRange; }
		/**
		 * Set the minimum range.
		 * The minimum range is the distance from the center of the audio source at which the sound is still hearable at full volume.
		 */
		void setMinimumRange(const float& r)
		{
			_minimumRange = r;
			alGetError();
			alSourcef(_handle, AL_REFERENCE_DISTANCE, _minimumRange);
			Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to set audio source minimum range. The change is likely not applied", AssertSeverity::Warning);
		}

		/**
		 * The maximum range is the distance from the center of the audio source at which the sound's volume reaches 0.
		 */
		[[nodiscard]] float maximumRange() const { return _maximumRange; }
		/**
		 * Set the maximum range.
		 * The maximum range is the distance from the center of the audio source at which the sound's volume reaches 0.
		 */
		void setMaximumRange(const float& r)
		{
			_maximumRange = r;
			alGetError();
			alSourcef(_handle, AL_MAX_DISTANCE, _maximumRange);
			Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to set audio source maximum range. The change is likely not applied", AssertSeverity::Warning);
		}

		/**
		 * The minimum volume prevents the volume from going lower than the set value, even when the listener is outside of the maximum range.
		 */
		[[nodiscard]] float minimumVolume() const { return _minimumVolume; }
		/**
		 * Set the minimum volume.
		 * The minimum volume prevents the volume from going lower than the set value, even when the listener is outside of the maximum range.
		 */
		void setMinimumVolume(const float& mv)
		{
			_minimumVolume = mv;
			alGetError();
			alSourcef(_handle, AL_MIN_GAIN, _minimumVolume);
			Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to set audio source minimum volume. The change is likely not applied", AssertSeverity::Warning);
		}

		/**
		 * If true, the audio clip loops back to the start when its done and continues playing indefinitely.
		 */
		[[nodiscard]] bool looping() const { return _looping; }
		/**
		 * Set if the audio source is looping or not.
		 * If true, the audio clip loops back to the start when its done and continues playing indefinitely.
		 */
		void setLooping(const bool& l)
		{
			_looping = l;
			alGetError();
			alSourcei(_handle, AL_LOOPING, _looping);
			Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to set audio source looping. The change is likely not applied", AssertSeverity::Warning);
		}

		/**
		 * The source's audio clip. This is the audio data that is being played from this source.
		 */
		[[nodiscard]] AudioClip* clip() const { return _clip; }
		/**
		 * Set the source's audio clip to the given clip.
		 * If the clip is null, the current clip is cleared and audio stops playing.
		 */
		void setClip(AudioClip* clip)
		{
			alGetError();
			_clip = clip;
			if (clip != nullptr)
				alSourcei(_handle, AL_BUFFER, clip->_buffer);
			else
			{
				alSourcei(_handle, AL_BUFFER, AL_NONE);
				alSourceStop(_handle);
			}
			Console::assertLog(alGetError() == AL_NO_ERROR, "Failed to set audio source's clip. The change is likely not applied", AssertSeverity::Warning);
		}

		/**
		 * Used to implement playOnStart.
		 */
		void start() override;
		/**
		 * Used to update the audio source's position.
		 */
		void lateUpdate() override;
	private:
		ALuint _handle = 0;

		float _pitch = 1;
		float _volume = 1;

		float _minimumRange = 0.0f;
		float _maximumRange = 256;
		
		float _minimumVolume = 0.0f;

		bool _looping = false;

		bool _playOnStart = false;

		AudioClip* _clip = nullptr;
		String _clipPath = "";
	};

	REGISTER_TYPE(AudioSource);
}