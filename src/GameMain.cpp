#ifndef TRISTEON_EDITOR
#include "Engine.h"
#include <ctime>

#include <GameWindow.h>

std::unique_ptr<Tristeon::Engine> engine;

int main(int argc, char** argv)
{
	srand(std::time(nullptr)); //TODO: Replace rand()

	Tristeon::GameWindow window;
	engine = std::make_unique<Tristeon::Engine>();
	engine->playMode(true);
	engine->run();
	return 0;
}
#endif