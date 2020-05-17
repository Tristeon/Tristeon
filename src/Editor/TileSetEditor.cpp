#include "TileSetTab.h"
#ifdef TRISTEON_EDITOR
#include "TileSetEditor.h"

namespace TristeonEditor
{
	void TileSetEditor::initialize()
	{
		contents = findChild<QWidget*>("tilesets_contents");
		tileSetTabs = new QTabWidget(contents);
		tileSetTabs->setStyleSheet("background-color: rgb(255, 255, 255);\nborder-color: rgb(0, 0, 0);\ncolor: rgb(00, 00, 00);");
		
		std::string path = (QDir::currentPath() + "/Project/TilesetTest.tileset").toStdString();
		auto testTab = new TileSetTab(path);
		
		tileSetTabs->addTab(testTab,"Test");

		auto layout = new QVBoxLayout();
		layout->addWidget(tileSetTabs);
		contents->setLayout(layout);
	}

	void TileSetEditor::sceneLoaded(Tristeon::Scene* scene)
	{
		
	}
}
#endif