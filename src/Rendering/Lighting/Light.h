#pragma once
#include <Scenes/Actors/Behaviour.h>
#include <Utils/Colour.h>
#include <Rendering/RenderMask.h>

namespace Tristeon
{
	class Light : public Behaviour
	{
	public:
		Light();
		virtual ~Light() override;

		[[nodiscard]] json serialize() override;
		void deserialize(json pJson) override;

		[[nodiscard]] float intensity() const { return _intensity; }
		void setIntensity(const float& pValue) { _intensity = pValue; }
		
		[[nodiscard]] Colour colour() const { return _colour; }
		void setColour(const Colour& pValue) { _colour = pValue; }
	protected:
		float _intensity = 1.0f;
		Colour _colour;
		RenderMask _mask = RenderMask::All;
	};
}
