#include "Editor.h"
#ifdef TRISTEON_EDITOR
#include <QtWidgets>
#include <Editor/FileExplorer.h>
#include <QApplication>
#include <QFileSystemModel>
#include <qheaderview.h>

namespace TristeonEditor
{
	void FileExplorer::initialize()
	{
		auto* contents = findChild<QWidget*>("fileexplorerContents");

		auto* grid = new QGridLayout(contents);
		grid->setContentsMargins(0, 0, 0, 0);
		contents->setLayout(grid);
		model = new QFileSystemModel(this);

		model->setRootPath(QApplication::applicationDirPath() + "/Project/");
		view = new QTreeView(this);
		view->setModel(model);
		view->show();
		grid->addWidget(view);
		view->setAnimated(true);
		view->setIndentation(20);
		view->setSortingEnabled(true);
		view->setRootIndex(model->index(QApplication::applicationDirPath() + "/Project/"));
		connect(view->selectionModel(), &QItemSelectionModel::selectionChanged, this, &FileExplorer::selectionChanged);
		
		auto* header = view->findChild<QHeaderView*>();
		if (header != nullptr)
			header->setStyleSheet("color: rgb(0, 0, 0);");
	}

	void FileExplorer::sceneLoaded(Tristeon::Scene* scene)
	{
		
	}

	void FileExplorer::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
	{
		QModelIndex const selection = selected.indexes()[0];
		QString const path = model->filePath(selection);

		QDir const baseDir(QDir::currentPath());
		QString const localPath = baseDir.relativeFilePath(path);

		QFile const file{ QString(localPath) };
		QFileInfo const info{ file };

		current.path = localPath.toStdString();
		current.extension = info.suffix().toStdString();
		current.name = info.baseName().toStdString();
		
		Editor::instance()->selectedFileItem(&current);
	}
}
#endif