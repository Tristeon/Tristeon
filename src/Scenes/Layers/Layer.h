#pragma once

namespace Tristeon
{
	class Renderer;
	class Scene;

	class Layer
	{
	public:
		virtual void render(Renderer* renderer, Scene* scene) = 0;
	};
}