#ifndef TRISTEON_EDITOR
#include "AssetManagement/AssetDatabase.h"
#include "Engine.h"
#include "GameWindow.h"

std::unique_ptr<Tristeon::Engine> engine;

int main(int argc, char** argv)
{
	Tristeon::Console::init();
	
	Tristeon::Settings::loadBuild();
	Tristeon::AssetDatabase::load();

	Tristeon::GameWindow window;
	engine = std::make_unique<Tristeon::Engine>();
	engine->setPlayMode(true);
	engine->run();
	return 0;
}
#endif