#pragma once
#include <Utils/Singleton.h>
#include "AL/alc.h"

namespace Tristeon
{
	class AudioClip;

	/**
	 * Audio manages the initialization of the Audio API, its only instance is owned and managed by the engine.
	 */
	class Audio final : private Singleton<Audio>
	{
		friend class Engine;
	public:
		Audio();
		~Audio();
	private:
		ALCdevice* _device = nullptr;
		ALCcontext* _context = nullptr;
	};
}