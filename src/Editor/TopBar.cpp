#ifdef TRISTEON_EDITOR
#include "TopBar.h"
#include <Engine.h>
#include <Scenes/SceneManager.h>
#include <QtWidgets>

namespace TristeonEditor
{
	TopBar::TopBar(QWidget* parent) : QDockWidget(parent)
	{
		auto* title = new QWidget;
		setTitleBarWidget(title);
		titleBarWidget()->hide();
	}

	void TopBar::initialize()
	{
		auto* play = findChild<QPushButton*>("start");
		connect(play, &QPushButton::clicked, this, &TopBar::startGame);
		
		auto* pause = findChild<QPushButton*>("pause");
		connect(pause, &QPushButton::clicked, this, &TopBar::pauseGame);

		auto* stop = findChild<QPushButton*>("stop");
		connect(stop, &QPushButton::clicked, this, &TopBar::stopGame);
	}

	void TopBar::loadScene(Tristeon::Scene* scene)
	{
		//Empty
	}

	void TopBar::startGame()
	{
		if (!Tristeon::Engine::playMode())
			Tristeon::Engine::playMode(true);
		else
			stopGame();
	}

	void TopBar::stopGame()
	{
		Tristeon::Engine::playMode(false);
		Tristeon::SceneManager::reload();
	}

	void TopBar::pauseGame()
	{
		Tristeon::Engine::playMode(false);
	}
}
#endif