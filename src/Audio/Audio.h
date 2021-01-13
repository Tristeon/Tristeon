#pragma once
#include <Utils/Singleton.h>

#include "AL/al.h"
#include "AL/alc.h"
#include "Math/Vector.h"
#include "Standard/String.h"

namespace Tristeon
{
	class AudioClip;

	/**
	 * Audio manages the initialization of the Audio API, its only instance is owned and managed by the engine.
	 */
	class Audio final : private Singleton<Audio>
	{
		friend Singleton<Audio>;
		friend class Engine;
	public:
		/**
		 * A handle that holds a reference to a temporary audio source.
		 * Can be used in various Audio:: static functions.
		 */
		struct Handle
		{
			ALuint idx = 0;
			bool operator==(const Handle& other) const { return idx == other.idx; }
		};
		
		Audio();
		~Audio();

		/**
		 * Play the audio file at the given filepath without attenuation.
		 * The Audio::Handle return value can be used in other static functions to interact with the temporary audio source.
		 */
		static Handle play(const String& path, const bool& looping = false, const float& volume = 1.0f);
		/**
		 * Play the audio file at the given filepath with attenuation using the given position and range.
		 * The Audio::Handle return value can be used in other static functions to interact with the temporary audio source.
		 */
		static Handle play(const String& path, const Vector& position, const float& minimumRange, const float& maximumRange, const bool& looping = false, const float& volume = 1.0f);

		/**
		 * Play the audio clip without attenuation.
		 * The Audio::Handle return value can be used in other static functions to interact with the temporary audio source.
		 */
		static Handle play(AudioClip* clip, const bool& looping = false, const float& volume = 1.0f);
		/**
		 * Play the audio clip with attenuation using the given position and range.
		 * The Audio::Handle return value can be used in other static functions to interact with the temporary audio source.
		 */
		static Handle play(AudioClip* clip, const Vector& position, const float& minimumRange, const float& maximumRange, const bool& looping = false, const float& volume = 1.0f);

		/**
		 * Returns true if the given audio handle is currently playing.
		 */
		[[nodiscard]] static bool isPlaying(const Handle& handle);

		/**
		 * Stops the audio handle and deletes the internal audio source.
		 * This also sets handle.idx to 0.
		 */
		static void stop(Handle& handle);

		/**
		 * Sets the position of the given audio handle. This has no effect on audio handles that were played without attenuation.
		 */
		static void setPosition(const Handle& handle, const Vector& position);
	private:
		/**
		 * Adds the source and sets a timer for when the source ought to be removed.
		 */
		static void addSource(const Handle& handle, const bool& looping);
		/**
		 * Removes the handle after n milliseconds.
		 */
		static void removeAfter(Handle handle);

		ALCdevice* _device = nullptr;
		ALCcontext* _context = nullptr;

		List<Handle> _sources;
	};

#ifdef TRISTEON_LOGENABLED
	// ReSharper disable once CppUnusedIncludeDirective
#include <Utils/Console.h>

#define AUDIO_ASSERT(function) \
{ \
	alGetError(); \
	function; \
	auto err = alGetError(); \
	TRISTEON_ASSERT(err == AL_NO_ERROR, Tristeon::String("Error ") + std::to_string(err) + " occured when calling function " + Tristeon::String(#function), Tristeon::AssertSeverity::Warning); \
}
#else
#define AUDIO_ASSERT(function)
	function;
#endif
}