#pragma once
#include "PointLight.h"
#include <Math/Vector.h>
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	class SpotLight : public PointLight
	{
	public:
		[[nodiscard]] Vector direction() const { return _direction; }
		void setDirection(const Vector& value) { _direction = value; }

		[[nodiscard]] float outerCutoff() const { return _outerCutoff; }
		void setOuterCutoff(const float& degrees) { _outerCutoff = degrees; }

		[[nodiscard]] float innerCutoff() const { return _innerCutoff; }
		void setInnerCutoff(const float& degrees) { _innerCutoff = degrees; }

		[[nodiscard]] json serialize() override;
		void deserialize(json j) override;
		void drawGizmos() override;

	protected:
		void prepareRender(Shader* shader, unsigned index) override;
		Vector _direction{};
		float _innerCutoff = 360.0f;
		float _outerCutoff = 360;
	};
	REGISTER_TYPE(SpotLight);
}
