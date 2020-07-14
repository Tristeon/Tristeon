#pragma once
#ifdef TRISTEON_EDITOR
#include "DockWidget.h"
#include <QtWidgets>

namespace TristeonEditor
{
	class TopBar : public QWidget, public DockWidget
	{
	public:
		explicit TopBar(QWidget* parent = nullptr);
	protected:
		void initialize() override;
		void sceneLoaded(Tristeon::Scene* scene) override;

		void startGame();
		void stopGame();
		void pauseGame();

		QPushButton* start = nullptr;
		QPushButton* pause = nullptr;
		QPushButton* stop = nullptr;
	};
}
#endif