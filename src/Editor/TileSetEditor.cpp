#ifdef TRISTEON_EDITOR
#include "AssetManagement/AssetDatabase.h"
#include "Project.h"
#include "TileSetTab.h"
#include "TileSetEditor.h"
#include <filesystem>
namespace TristeonEditor
{
	void TileSetEditor::initialize()
	{
		contents = findChild<QWidget*>("tilesets_contents");
		tileSetTabs = new QTabWidget(contents);
		tileSetTabs->setStyleSheet(
			"background-color: rgb(255, 255, 255);\nborder-color: rgb(0, 0, 0);\ncolor: rgb(00, 00, 00);");

		for (auto tileset : Tristeon::AssetDatabase::get(".tileset"))
		{
			auto* tab = new TileSetTab(tileset);
			tileSetTabs->addTab(tab, QString::fromStdString(std::filesystem::path(tileset).stem().string()));
		}

		auto layout = new QVBoxLayout();
		layout->addWidget(tileSetTabs);
		contents->setLayout(layout);
	}

	void TileSetEditor::sceneLoaded(Tristeon::Scene* scene)
	{
	}
}
#endif