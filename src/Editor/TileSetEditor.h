#pragma once
#ifdef TRISTEON_EDITOR
#include "DockWidget.h"
#include <QtWidgets>

namespace TristeonEditor
{
	class TileSetEditor : public QDockWidget, public DockWidget
	{
	public:
		explicit TileSetEditor(QWidget* parent = nullptr) : QDockWidget(parent) {}
	protected:
		void initialize() override;
		void sceneLoaded(Tristeon::Scene* scene) override;

	private:
		QTabWidget* tileSetTabs = nullptr;
		QWidget* contents = nullptr;
	};
}
#endif