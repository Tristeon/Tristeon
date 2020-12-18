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

	private:
		ALuint _buffer = 0;
	};
}
