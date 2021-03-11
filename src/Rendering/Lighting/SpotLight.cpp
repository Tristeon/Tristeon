#include "SpotLight.h"

#include "Math/Math.h"
#include "Rendering/Gizmos.h"
#include "Rendering/Shader.h"

namespace Tristeon
{
	json SpotLight::serialize()
	{
		json j = PointLight::serialize();
		j["typeID"] = Type<SpotLight>::fullName();
		j["innerCutoff"] = _innerCutoff;
		j["outerCutoff"] = _outerCutoff;
		j["direction"] = _direction;
		return j;
	}

	void SpotLight::deserialize(json j)
	{
		PointLight::deserialize(j);

		_innerCutoff = j.value("innerCutoff", 360.0f);
		_outerCutoff = j.value("outerCutoff", 360.0f);
		_direction = j.value("direction", Vector{});
	}

	void SpotLight::drawGizmos()
	{
		PointLight::drawGizmos();
		Gizmos::drawLine(actor()->position, actor()->position + _direction.normalize() * _outerRadius, Colour::blue());
	}

	void SpotLight::prepareRender(Shader* shader, unsigned index)
	{
		shader->setUniformValue("spotLights[" + std::to_string(index) + "]" + ".intensity", intensity());
		shader->setUniformValue("spotLights[" + std::to_string(index) + "]" + ".color", _colour.r, _colour.g, _colour.b);
		shader->setUniformValue("spotLights[" + std::to_string(index) + "]" + ".position", actor()->position.x, actor()->position.y, -distance());

		shader->setUniformValue("spotLights[" + std::to_string(index) + "]" + ".innerRadius", innerRadius());
		shader->setUniformValue("spotLights[" + std::to_string(index) + "]" + ".outerRadius", outerRadius());

		auto d = _direction.normalize().rotate(-actor()->rotation);
		const auto invertedDirection = -d.normalize();
		shader->setUniformValue("spotLights[" + std::to_string(index) + "]" + ".invertedDirection", invertedDirection.x, invertedDirection.y);
		shader->setUniformValue("spotLights[" + std::to_string(index) + "]" + ".innerCutoff", cos(Math::toRadians(innerCutoff())));
		shader->setUniformValue("spotLights[" + std::to_string(index) + "]" + ".outerCutoff", cos(Math::toRadians(outerCutoff())));
	}
}
