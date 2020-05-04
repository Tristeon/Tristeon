#pragma once
#ifdef TRISTEON_EDITOR
#include "Dynamic/Files/FileItem.h"
#include <QFileSystemModel>
#include "EditorWindow.h"
#include <QDockWidget>
#include <qtreeview.h>

namespace TristeonEditor
{
	class FileExplorer : public QDockWidget, public EditorWindow
	{
	public:
		explicit FileExplorer(QWidget* parent = nullptr) : QDockWidget(parent) {}
		void initialize() override;
		void sceneLoaded(Tristeon::Scene* scene) override;
	private:
		void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

		FileItem current;
		QFileSystemModel* model = nullptr;
		QTreeView* view = nullptr;
	};
}
#endif