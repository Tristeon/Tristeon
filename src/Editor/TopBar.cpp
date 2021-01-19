#ifdef TRISTEON_EDITOR
#include "GameView.h"
#include "Window.h"
#include "TopBar.h"
#include <Engine.h>
#include <Scenes/SceneManager.h>
#include <Scenes/Scene.h>
#include <QtWidgets>

namespace TristeonEditor
{
	TopBar::TopBar(QWidget* parent) : QWidget(parent)
	{
	}

	void TopBar::initialize()
	{
		start = findChild<QPushButton*>("start");
		connect(start, &QPushButton::clicked, this, &TopBar::startGame);
		
		pause = findChild<QPushButton*>("pause");
		connect(pause, &QPushButton::clicked, this, &TopBar::pauseGame);

		stop = findChild<QPushButton*>("stop");
		connect(stop, &QPushButton::clicked, this, &TopBar::stopGame);

		QShortcut* shortcut = new QShortcut(QKeySequence(tr("Ctrl+S", "File|Save")), this);
		connect(shortcut, &QShortcut::activated, this, [&]() { Tristeon::SceneManager::saveCurrent(); });
	}

	void TopBar::sceneLoaded(Tristeon::Scene* scene)
	{
		//Empty
	}

	void TopBar::startGame()
	{
		if (!Tristeon::Engine::playMode())
		{
			GameView::instance()->setFocus();
			if (!pause->isChecked())
			{
				sceneBeforePlay = Tristeon::SceneManager::current()->serialize();
				sceneBeforePlay["path"] = Tristeon::SceneManager::current()->path();

				Tristeon::SceneManager::load(sceneBeforePlay); //Reload scene
			}
			Tristeon::Engine::setPlayMode(true);
			pause->setChecked(false);
			start->setChecked(true);
		}
		else
			stopGame();
	}

	void TopBar::stopGame()
	{
		Tristeon::Engine::setPlayMode(false);
		Tristeon::SceneManager::reload();

		start->setChecked(false);
		pause->setChecked(false);

		Tristeon::SceneManager::load(sceneBeforePlay);
	}

	void TopBar::pauseGame()
	{
		if (!start->isChecked())
		{
			pause->setChecked(false);
			return;
		}

		Tristeon::Engine::setPlayMode(!pause->isChecked());
		if (!pause->isChecked())
			GameView::instance()->setFocus();
	}
}
#endif