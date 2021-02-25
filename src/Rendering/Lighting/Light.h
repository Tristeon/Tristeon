#pragma once
#include <Scenes/Actors/Behaviour.h>
#include <Serialization/TypeRegister.h>
#include <Utils/Colour.h>
#include <Callbacks/IDrawGizmos.h>

namespace Tristeon
{
	class Light : public Behaviour, public IDrawGizmos
	{
		enum class SourceType : int
		{
			Point,
			Spot,
			Ambient
		};
		
	public:
		Light();
		virtual ~Light() override;

		[[nodiscard]] json serialize() override;
		void deserialize(json j) override;

		[[nodiscard]] SourceType type() const { return _type; }
		[[nodiscard]] float intensity() const { return _intensity; }
		[[nodiscard]] float innerRadius() const { return _innerRadius; }
		[[nodiscard]] float outerRadius() const { return _outerRadius; }
		[[nodiscard]] Colour colour() const { return _colour; }

		/**
		 * Only applies to Directional and Spot lights
		 */
		[[nodiscard]] Vector direction() const { return _direction; }
		/**
		 * Only applies to Spot lights
		 */
		[[nodiscard]] float outerCutoff() const { return _outerCutoff; }
		[[nodiscard]] float innerCutoff() const { return _innerCutoff; }

		[[nodiscard]] float distance() const { return _distance; }

		void drawGizmos() override;
	private:
		SourceType _type = SourceType::Point;
		float _intensity = 1.0f;

		float _innerRadius = 256.0f;
		float _outerRadius = 1028.0f;
		Colour _colour;

		Vector _direction{};
		float _innerCutoff = 360.0f;
		float _outerCutoff = 360;

		float _distance = 1;
	};

	REGISTER_TYPE(Light);
}
