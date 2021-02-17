#pragma once
#include "Behaviour.h"
#include <Serialization/TypeRegister.h>
#include <Utils/Colour.h>

namespace Tristeon
{
	class Light : public Behaviour
	{
		enum class SourceType : int
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

		[[nodiscard]] SourceType type() const { return _type; }
		[[nodiscard]] float intensity() const { return _intensity; }
		[[nodiscard]] float range() const { return _range; }
		[[nodiscard]] Colour colour() const { return _colour; }
	private:
		SourceType _type = SourceType::Point;
		float _intensity = 1.0f;
		float _range = 256.0f;
		Colour _colour;
	};

	REGISTER_TYPE(Light);
}
