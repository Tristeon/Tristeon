#ifdef TRISTEON_EDITOR
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

		std::string path = (QDir::currentPath() + "/Project/ParalaxTiles.tileset").toStdString();
		auto testTab = new TileSetTab(path);

		std::string path2 = (QDir::currentPath() + "/Project/Tiles.tileset").toStdString();
		auto testTab2 = new TileSetTab(path2);

		std::string path3 = (QDir::currentPath() + "/Project/SolidTiles.tileset").toStdString();
		auto testTab3 = new TileSetTab(path3);

		tileSetTabs->addTab(testTab, "Paralax Tiles");
		tileSetTabs->addTab(testTab2, "Tiles");
		tileSetTabs->addTab(testTab3, "Solid Tiles");

		auto layout = new QVBoxLayout();
		layout->addWidget(tileSetTabs);
		contents->setLayout(layout);
	}

	void TileSetEditor::sceneLoaded(Tristeon::Scene* scene)
	{
	}
}
#endif
