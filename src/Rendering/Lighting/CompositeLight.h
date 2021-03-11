#pragma once
#include "Light.h"
#include "Rendering/Shader.h"

namespace Tristeon
{
	class CompositeLight : public Light
	{
		friend class Renderer;

	public:
		CompositeLight();
		virtual ~CompositeLight() override;
	
	protected:
		virtual void render() = 0;
		virtual Shader* shader() = 0;
	};
}