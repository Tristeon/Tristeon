#include "DirectoryViewer.h"
#include <qheaderview.h>
#include <QLayout>

#include "Project.h"
#include "Editor/Editor.h"

using namespace TristeonEditor;
using namespace Tristeon;

namespace fs = std::filesystem;

DirectoryViewer::DirectoryViewer(QWidget* parent) : QTreeView(parent)
{
	watcher = new QFileSystemWatcher(this);

	model = new QFileSystemModel(this);

	auto parentPath = fs::path(Project::assetPath()).parent_path().parent_path();
	auto projectPath = QString::fromStdString(parentPath.string());
	model->setRootPath(projectPath);
	model->setFilter(QDir::Dirs | QDir::Drives | QDir::NoDotAndDotDot | QDir::AllDirs);

	auto* layout = new QGridLayout(this);
	setLayout(layout);


	QTreeView::setModel(model);
	show();

	//Hide unnecessary columns
	for (int i = 1; i < model->columnCount(); ++i)
		hideColumn(i);

	setHeaderHidden(true);
	setAnimated(true);
	setIndentation(20);
	setSortingEnabled(true);

	QTreeView::setRootIndex(model->index(projectPath));

	//Hide folders that are not the asset folder
	for (int i = 0; i < model->rowCount(); ++i)
	{
		//QModelIndex index = QTreeView::indexAt(QPoint(i, 0));
		auto index = model->filePath(rootIndex()).toStdString();

		auto variant = model->itemData(model->index(projectPath))[i];
		auto string = variant.toString().toStdString();

		if (string == "Project")
			continue;

		setRowHidden(i, rootIndex(), true);
	}
	

	auto* header = findChild<QHeaderView*>();
	if (header != nullptr)
		header->setStyleSheet("color: rgb(0, 0, 0);");
}