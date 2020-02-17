#include "Engine.h"

#include <QApplication>

namespace Tristeon
{
	Tristeon::Engine::Engine()
	{
		window = std::make_unique<Window>(this);
		renderer = std::make_unique<Renderer>(this);
	}

	void Engine::run()
	{
		QApplication::exec();
	}
}
