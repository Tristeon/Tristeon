#pragma once
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QObject>
namespace Tristeon
{
	class Window;
	class Engine;
	
	class GLContext : public QObject, public QOpenGLFunctions
	{
		friend Engine;
		friend Window;
	public:
		GLContext(Window* window);
		~GLContext();
		static bool isReady() { return instance != nullptr; }
		static GLContext* getInstance();
		
		static QOpenGLContext* getQContext();
		static Window* getWindow();

		static void makeCurrent();
	private:
		static GLContext* instance;
		QOpenGLContext* context = nullptr;
		Window* window = nullptr;
	};
}
