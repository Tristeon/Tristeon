#pragma once
#ifdef TRISTEON_EDITOR
#include "DockWidget.h"
#include <QtWidgets>

namespace TristeonEditor
{
	class TilesetEditor : public QDockWidget, public DockWidget
	{
	public:
		explicit TilesetEditor(QWidget* parent = nullptr) : QDockWidget(parent) {}
	protected:
		void initialize() override;
		void sceneLoaded(Tristeon::Scene* scene) override;

	private:
		QTabWidget* tilesetTabs = nullptr;
		QWidget* contents = nullptr;
	};
}
#endif