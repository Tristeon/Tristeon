#ifdef TRISTEON_EDITOR
#include "AssetManagement/AssetDatabase.h"
#include "Settings.h"
#include "TilesetTab.h"
#include "TilesetEditor.h"
#include <filesystem>
namespace TristeonEditor
{
	void TilesetEditor::initialize()
	{
		contents = findChild<QWidget*>("tilesets_contents");
		tilesetTabs = new QTabWidget(contents);
		tilesetTabs->setStyleSheet(
			"background-color: rgb(255, 255, 255);\nborder-color: rgb(0, 0, 0);\ncolor: rgb(00, 00, 00);");

		for (auto tileset : Tristeon::AssetDatabase::get(".tileset"))
		{
			auto* tab = new TilesetTab(tileset);
			tilesetTabs->addTab(tab, QString::fromStdString(std::filesystem::path(tileset).stem().string()));
		}

		auto layout = new QVBoxLayout();
		layout->addWidget(tilesetTabs);
		contents->setLayout(layout);
	}

	void TilesetEditor::sceneLoaded(Tristeon::Scene* scene)
	{
	}
}
#endif