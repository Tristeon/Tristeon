#pragma once

namespace Tristeon
{
	class Scene;

	class Layer
	{
	public:
		virtual void render(Scene* scene) = 0;
	};
}