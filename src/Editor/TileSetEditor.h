#pragma once
#ifdef TRISTEON_EDITOR
#include <QtWidgets>
#include "EditorWindow.h"

namespace TristeonEditor
{
	class TileSetEditor : public QDockWidget, public EditorWindow
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