#pragma once

#include <QOpenGLWidget>

namespace Tristeon
{
	class Engine;

	class Window : QOpenGLWidget
	{
	public:
		explicit Window(Engine* engine);
		~Window();

	private:
		void onResize(int w, int h);

		int width = 0, height = 0;
		Engine* engine = nullptr;
	};
}