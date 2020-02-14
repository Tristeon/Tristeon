#pragma once

struct GLFWwindow;

namespace Tristeon
{
	class Engine;

	class Window final
	{
	public:
		explicit Window(Engine* engine);
		~Window();

		GLFWwindow* getGLFWwindow() const { return window; }
	private:
		void onResize(int w, int h);

		GLFWwindow* window = nullptr;

		int width = 0, height = 0;
		Engine* engine = nullptr;
	};
}