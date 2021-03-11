#pragma once
#include "Light.h"
#include "Callbacks/IDrawGizmos.h"

namespace Tristeon
{
	class Shader;

	class PointLight : public Light, public IDrawGizmos
	{
		friend class Renderer;
	public:
		PointLight();
		virtual ~PointLight() override;
		
		[[nodiscard]] json serialize() override;
		void deserialize(json j) override;

		[[nodiscard]] float innerRadius() const { return _innerRadius; }
		void setInnerRadius(const float& value) { _innerRadius = value; }

		[[nodiscard]] float outerRadius() const { return _outerRadius; }
		void setOuterRadius(const float& value) { _outerRadius = value; }

		[[nodiscard]] float distance() const { return _distance; }
		void setDistance(const float& value) { _distance = value; }

		void drawGizmos() override;

	protected:
		virtual void prepareRender(Shader* shader, unsigned int index);
		
		float _innerRadius = 256.0f;
		float _outerRadius = 1028.0f;

		float _distance = 1;
	};
	REGISTER_TYPE(PointLight);
}