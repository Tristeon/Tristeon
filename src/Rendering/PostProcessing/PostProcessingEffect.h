#pragma once
#include <Scenes/Actors/Behaviour.h>
#include <Rendering/Shader.h>

namespace Tristeon
{
	class PostProcessingEffect : public Behaviour
	{
	public:
		virtual void render();
		virtual Shader* shader() = 0;
	};
}