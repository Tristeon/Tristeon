#include "TileSetTab.h"
#ifdef TRISTEON_EDITOR
#include "TileSetEditor.h"

namespace TristeonEditor
{
	void TileSetEditor::initialize()
	{
		auto tileSetTabs = new QTabWidget(this);

		auto testTab = new TileSetTab(nullptr,"");
		tileSetTabs->addTab(testTab,"Test");
	}

	void TileSetEditor::sceneLoaded(Tristeon::Scene* scene)
	{
		
	}
}
#endif