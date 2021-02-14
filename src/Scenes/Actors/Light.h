#pragma once
#include "Behaviour.h"

#include <Serialization/TypeRegister.h>
namespace Tristeon
{
	class Light : public Behaviour
	{
		enum class SourceType : char
		{
			Point,
			Spot,
			Directional
		};
		
	public:
		Light();
		virtual ~Light() override;

		[[nodiscard]] json serialize() override;
		void deserialize(json j) override;

	private:
		SourceType _type = SourceType::Point;
		float _intensity = 1.0f;
		float _range = 256.0f;
	};

	REGISTER_TYPE(Light);
}