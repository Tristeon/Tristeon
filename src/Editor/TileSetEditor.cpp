#ifdef TRISTEON_EDITOR
#include "Project.h"
#include "TileSetTab.h"
#include "TileSetEditor.h"

namespace TristeonEditor
{
	void TileSetEditor::initialize()
	{
		contents = findChild<QWidget*>("tilesets_contents");
		tileSetTabs = new QTabWidget(contents);
		tileSetTabs->setStyleSheet(
			"background-color: rgb(255, 255, 255);\nborder-color: rgb(0, 0, 0);\ncolor: rgb(00, 00, 00);");

		//std::string const path = "TilesetTest.tileset";
		//auto testTab = new TileSetTab(path);

		//std::string path2 = "GoldBricks.tileset";
		//auto testTab2 = new TileSetTab(path2);

		//tileSetTabs->addTab(testTab, "Tileset Test");
		//tileSetTabs->addTab(testTab2, "Gold Bricks");

		auto layout = new QVBoxLayout();
		layout->addWidget(tileSetTabs);
		contents->setLayout(layout);
	}

	void TileSetEditor::sceneLoaded(Tristeon::Scene* scene)
	{
	}
}
#endif
