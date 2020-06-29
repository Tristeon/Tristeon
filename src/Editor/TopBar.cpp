#ifdef TRISTEON_EDITOR
#include "GameView.h"
#include "Window.h"
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
				Tristeon::SceneManager::saveCurrent();
				Tristeon::SceneManager::reload();
			}
			Tristeon::Engine::playMode(true);
			pause->setChecked(false);
			start->setChecked(true);

			Tristeon::Renderer::showGrid = false;
		}
		else
			stopGame();
	}

	void TopBar::stopGame()
	{
		Tristeon::Engine::playMode(false);
		Tristeon::SceneManager::reload();

		start->setChecked(false);
		pause->setChecked(false);

		Tristeon::Renderer::showGrid = true;
	}

	void TopBar::pauseGame()
	{
		if (!start->isChecked())
		{
			pause->setChecked(false);
			return;
		}

		Tristeon::Engine::playMode(!pause->isChecked());
		if (!pause->isChecked())
			GameView::instance()->setFocus();

		Tristeon::Renderer::showGrid = pause->isChecked();
	}
}
#endif