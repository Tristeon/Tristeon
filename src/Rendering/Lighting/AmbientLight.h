#pragma once
#include "Light.h"
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	class AmbientLight : public Light
	{
	public:
		AmbientLight();
		virtual ~AmbientLight() override;
		
		[[nodiscard]] json serialize() override;
		void deserialize(json j) override;
	};
	REGISTER_TYPE(AmbientLight);
}