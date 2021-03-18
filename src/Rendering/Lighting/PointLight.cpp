#include "PointLight.h"
#include <Rendering/Gizmos.h>
#include <Rendering/Shader.h>
#include <Scenes/Actors/Actor.h>

namespace Tristeon
{
	PointLight::PointLight()
	{
		Collector<PointLight>::add(this);
	}

	PointLight::~PointLight()
	{
		Collector<PointLight>::remove(this);
	}

	json PointLight::serialize()
	{
		json j = Light::serialize();
		j["typeID"] = Type<PointLight>::fullName();
		j["innerRadius"] = _innerRadius;
		j["outerRadius"] = _outerRadius;
		j["distance"] = _distance;
		return j;
	}

	void PointLight::deserialize(json j)
	{
		Light::deserialize(j);

		_innerRadius = j.value("innerRadius", 256.0f);
		_outerRadius = j.value("outerRadius", 1028.0f);
		_distance = j.value("distance", 1.0f);
	}

	void PointLight::drawGizmos()
	{
		Gizmos::drawCircle(actor()->position, _innerRadius, Colour::green());
		Gizmos::drawCircle(actor()->position, _outerRadius, Colour::red());
	}

	void PointLight::prepareRender(Shader* shader, unsigned index)
	{
		shader->setUniformValue("pointLights[" + std::to_string(index) + "]" + ".intensity", intensity());
		shader->setUniformValue("pointLights[" + std::to_string(index) + "]" + ".color", _colour.r, _colour.g, _colour.b);
		shader->setUniformValue("pointLights[" + std::to_string(index) + "]" + ".position", actor()->position.x, actor()->position.y, -distance());

		shader->setUniformValue("pointLights[" + std::to_string(index) + "]" + ".innerRadius", innerRadius());
		shader->setUniformValue("pointLights[" + std::to_string(index) + "]" + ".outerRadius", outerRadius());
	}
}
