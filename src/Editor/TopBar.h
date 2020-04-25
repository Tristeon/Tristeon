#pragma once
#ifdef TRISTEON_EDITOR
#include "EditorWindow.h"
#include <QtWidgets>

namespace TristeonEditor
{
	class TopBar : public QDockWidget, public EditorWindow
	{
	public:
		explicit TopBar(QWidget* parent = nullptr);
	protected:
		void initialize() override;
		void loadScene(Tristeon::Scene* scene) override;

		void startGame();
		void stopGame();
		void pauseGame();

		QPushButton* start = nullptr;
		QPushButton* pause = nullptr;
		QPushButton* stop = nullptr;
	};
}
#endif