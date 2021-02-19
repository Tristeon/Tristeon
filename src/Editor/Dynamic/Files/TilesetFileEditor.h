#pragma once
#ifdef TRISTEON_EDITOR
#include "JsonFileEditor.h"
#include "FileEditorRegister.h"
#include "Scenes/Tiles/Tileset.h"

namespace TristeonEditor
{
	class TilesetFileEditor : public JsonFileEditor
	{
		FILE_EDITOR_H("tileset", TilesetFileEditor)
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
		
		void changeTexturePressed(const Tristeon::String& idx, QPushButton* button);

		QLabel* image = nullptr;
		QLabel* tileHighlight = nullptr;
		int selectedTile = -1;
		Tristeon::TileInfo selectedTileInfo{};
		QFrame* selectedTileData = nullptr;

		Tristeon::Tileset* set = nullptr;
	};
}
#endif