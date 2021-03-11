#pragma once
#include <Scenes/Actors/Behaviour.h>
#include <Serialization/TypeRegister.h>
#include <Utils/Colour.h>
#include <set>

namespace Tristeon
{
	class Light : public Behaviour
	{
	public:
		Light();
		virtual ~Light() override;

		[[nodiscard]] json serialize() override;
		void deserialize(json j) override;

		[[nodiscard]] float intensity() const { return _intensity; }
		void setIntensity(const float& value) { _intensity = value; }
		
		[[nodiscard]] Colour colour() const { return _colour; }
		void setColour(const Colour& value) { _colour = value; }
		
		void ignore(Layer* layer);
		void unignore(Layer* layer);
		std::set<Layer*> ignored();
	protected:
		float _intensity = 1.0f;
		Colour _colour;

		std::set<uint32_t> _ignoreLayers;
	};
}