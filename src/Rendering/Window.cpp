#include "Window.h"

#include "Engine.h"

namespace Tristeon
{
	Window::Window(Engine* engine) : engine(engine)
	{
		show();
	}

	Window::~Window()
	{
	}

	void Window::onResize(int w, int h)
	{
		width = w;
		height = h;
	}
}