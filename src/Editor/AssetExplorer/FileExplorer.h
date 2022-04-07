#pragma once
#ifdef TRISTEON_EDITOR
#include "../DockWidget.h"
#include <QDockWidget>

namespace TristeonEditor
{
	class FileExplorer : public QDockWidget, public DockWidget
	{
	public:
		explicit FileExplorer(QWidget* parent = nullptr) : QDockWidget(parent) {}
		void initialize() override;
	protected:
		void sceneLoaded(Tristeon::Scene* scene) override {}
	};
}
#endif