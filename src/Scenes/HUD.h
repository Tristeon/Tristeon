#pragma once
#include <Serialization/Serializable.h>
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	/**
	 * HUD is still a work in progress and currently contains no further implementation.
	 */
	class HUD : public Serializable
	{
		REGISTER_TYPE_H(HUD)
		//TODO: HUD design and implementation
	public:
		json serialize() override;
		void deserialize(json j) override;
	};
}
