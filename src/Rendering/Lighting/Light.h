#pragma once
#include <Scenes/Actors/Behaviour.h>
#include <Serialization/TypeRegister.h>
#include <Utils/Colour.h>
#include <Callbacks/IDrawGizmos.h>
#include <set>

namespace Tristeon
{
	class Light : public Behaviour, public IDrawGizmos
	{
	public:
		enum class SourceType : int
		{
			Point,
			Spot,
			Ambient,
			EndOfEnum
		};

		Light();
		virtual ~Light() override;

		[[nodiscard]] json serialize() override;
		void deserialize(json j) override;

		[[nodiscard]] SourceType type() const { return _type; }
		void setType(const SourceType& value) { _type = value; }

		[[nodiscard]] float intensity() const { return _intensity; }
		void setIntensity(const float& value) { _intensity = value; }
		
		[[nodiscard]] float innerRadius() const { return _innerRadius; }
		void setInnerRadius(const float& value) { _innerRadius = value; }
		
		[[nodiscard]] float outerRadius() const { return _outerRadius; }
		void setOuterRadius(const float& value) { _outerRadius = value; }
		
		[[nodiscard]] Colour colour() const { return _colour; }
		void setColour(const Colour& value) { _colour = value; }

		/**
		 * Only applies to Spot lights
		 */
		[[nodiscard]] Vector direction() const { return _direction; }
		void setDirection(const Vector& value) { _direction = value; }

		/**
		 * Only applies to Spot lights, in degrees.
		 */
		[[nodiscard]] float outerCutoff() const { return _outerCutoff; }
		void setOuterCutoff(const float& degrees) { _outerCutoff = degrees; }
		
		[[nodiscard]] float innerCutoff() const { return _innerCutoff; }
		void setInnerCutoff(const float& degrees) { _innerCutoff = degrees; }
		
		[[nodiscard]] float distance() const { return _distance; }
		void setDistance(const float& value) { _distance = value; }
		
		void ignore(Layer* layer);
		void unignore(Layer* layer);
		std::set<Layer*> ignored();

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

		std::set<uint32_t> _ignoreLayers;
	};

	REGISTER_TYPE(Light);
}