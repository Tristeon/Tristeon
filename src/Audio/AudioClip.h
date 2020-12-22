#pragma once
#include <TObject.h>
#include <Standard/String.h>

#include "AL/al.h"

namespace Tristeon
{
	/**
	 * Represents audio data, either loaded from a file, or user generated.
	 */
	class AudioClip : public TObject
	{
		friend class AudioSource;
		friend class AudioListener;
		friend class Audio;

	public:
		/**
		 * Creates an empty audioclip.
		 */
		AudioClip();
		~AudioClip();

		/**
		 * Load audio files from a given path.
		 * You may use this constructor if you only plan on using the audioclip once.
		 * If an audioclip is loaded multiple times, Resources::assetLoad is recommended.
		 */
		explicit AudioClip(const String& path);

		/**
		 * Create an audio clip using a given buffer, size and frequency.
		 * You may use this constructor if you wish to load or generate audio data manually.
		 *
		 * The constructor doesn't take ownership of the supplied data and destruction still has to be done manually.
		 */
		explicit AudioClip(void* data, const unsigned int& size, const unsigned int& frequency);

	private:
		ALuint _buffer = 0;
	};
}
