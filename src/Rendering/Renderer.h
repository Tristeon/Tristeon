#pragma once

namespace Tristeon
{
	class Engine;

	class Renderer
	{
	public:
		Renderer(Engine* engine);
		~Renderer();

		void renderScene();
		void renderHUD();
	private:
		Engine* engine = nullptr;
	};
}