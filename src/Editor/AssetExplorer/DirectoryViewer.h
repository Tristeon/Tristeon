#pragma once
#include <QFileSystemModel>
#include <qfilesystemwatcher.h>
#include <qtreeview.h>

#include "Editor/Dynamic/Files/FileItem.h"

namespace TristeonEditor
{
	class DirectoryViewer : public QTreeView
	{
	public:
		explicit DirectoryViewer(QWidget* parent);

		QFileSystemModel* getModel() { return model; }
	private:
		QFileSystemModel* model = nullptr;
		FileItem current;
		QFileSystemWatcher* watcher = nullptr;
	};
}
