#include <Editor/AssetExplorer/FileExplorer.h>
#include "DirectoryViewer.h"
#include "ContentViewer.h"
#include "Project.h"
#ifdef TRISTEON_EDITOR
#include "../Editor.h"
#include <QtWidgets>

using namespace TristeonEditor;

void FileExplorer::initialize()
{
	auto* contents = findChild<QWidget*>("fileexplorerContents");

	auto* layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	contents->setLayout(layout);

	auto* splitter = new QSplitter(this);
	layout->addWidget(splitter);

	auto* directoryViewer = new DirectoryViewer(this);
	splitter->addWidget(directoryViewer);

	auto* contentViewer = new ContentViewer(this);
	splitter->addWidget(contentViewer);

	connect(directoryViewer->selectionModel(), &QItemSelectionModel::selectionChanged, contentViewer,
		[=](const QItemSelection& selected, const QItemSelection& deselected)
		{
			QModelIndex selection = selected.indexes()[0];
			QString path = directoryViewer->getModel()->filePath(selection);
			
			QDir const baseDir(Tristeon::Project::assetPath().c_str());
			QString const localPath = baseDir.relativeFilePath(path);
			
			contentViewer->setDirectory(path);
		});
}
#endif