#include "ContentViewer.h"
#include "Project.h"
#include "Utils/Console.h"

using namespace TristeonEditor;
using namespace Tristeon;

ContentViewer::ContentViewer(QWidget* parent) : QListView(parent)
{
	setFlow(LeftToRight);
	setResizeMode(Adjust);
	setGridSize(QSize(64, 64));
	setSpacing(5);
	setViewMode(IconMode);

	model = new QFileSystemModel();
	model->setRootPath(QString::fromStdString(Project::assetPath()));

	QAbstractItemView::setModel(model);
	
	QListView::setRootIndex(model->index(QString::fromStdString(Project::assetPath())));

	show();
}

void ContentViewer::setDirectory(QString directory)
{
	Console::write(directory.toStdString());
	model->setRootPath(directory);
	QListView::setRootIndex(model->index(directory));
}