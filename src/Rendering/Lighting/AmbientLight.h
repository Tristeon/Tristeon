#pragma once
#include "Light.h"

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