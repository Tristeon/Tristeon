#pragma once
#include "Scenes/Actors/Behaviour.h"
#include "Utils/Singleton.h"
#include "Callbacks/ILateUpdate.h"

#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	/**
	 * The AudioListener is the in-game representation of the user's ear (hence listener).
	 * Its position determines the attenuation of audiosources and their stereo representation.
	 */
	class AudioListener : public Behaviour, public Singleton<AudioListener>, public ILateUpdate
	{
	public:
		[[nodiscard]] json serialize() override;
		void deserialize(json j) override;
		void lateUpdate() override;
	};

	REGISTER_TYPE(AudioListener);
}