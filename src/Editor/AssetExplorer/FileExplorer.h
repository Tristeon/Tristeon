#pragma once
#ifdef TRISTEON_EDITOR
#include "../DockWidget.h"
#include "../Dynamic/Files/FileItem.h"
#include <QFileSystemModel>
#include <QDockWidget>
#include <qtreeview.h>

namespace TristeonEditor
{
	class FileExplorer : public QDockWidget, public DockWidget
	{
	public:
		explicit FileExplorer(QWidget* parent = nullptr) : QDockWidget(parent) {}
		void initialize() override;
		void sceneLoaded(Tristeon::Scene* scene) override;
	private:
		void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

		FileItem current;
		QFileSystemModel* model = nullptr;
		QFileSystemWatcher* watcher = nullptr;
		QTreeView* view = nullptr;
	};
}
#endif