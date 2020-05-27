#pragma once
#ifdef TRISTEON_EDITOR
#include "JsonFileEditor.h"
#include "FileEditorRegister.h"
#include "FileTypes/TileSet.h"

namespace TristeonEditor
{
	class TileSetFileEditor : public JsonFileEditor
	{
		FILE_EDITOR_H("tileset", TileSetFileEditor)
	public:
		void initialize() override;
		void saveData() override;
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
	private:
		void loadTileset();
		void mousePressEvent(QMouseEvent* event) override;
		void saveCurrentTile();

		void selectedTileChanged();
		void tilesetChanged();
		
		QLabel* image = nullptr;
		QLabel* tileHighlight = nullptr;
		int selectedTile = -1;
		Tristeon::TileInfo selectedTileInfo{};
		QFrame* selectedTileData = nullptr;

		Tristeon::TileSet* set = nullptr;
	};
}
#endif