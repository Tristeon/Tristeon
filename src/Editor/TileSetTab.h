#pragma once
#ifdef TRISTEON_EDITOR

#include <qboxlayout.h>
#include <qlabel.h>
#include <QWidget>

#include "TypeDefinitions.h"
#include "FileTypes/TileSet.h"
#include "Scenes/Layers/Tile.h"

namespace TristeonEditor
{
	class TileSetTab : public QWidget
	{
	public:
		///Specify the path of the tileset png. It doesn't allow you to set a parent as tabs that get added to a QTabWidget shouldn't have a parent when constructed as they get added later
		explicit TileSetTab(const Tristeon::String& tileSetPath);

	private:
		void mousePressEvent(QMouseEvent* event) override;

		///Sets the selected tile by index. If the same index that has already highlighted is passed it will unhighlight it
		void setSelectedTile(int tileIndex);

		///Removes the highlight on the selected tile and sets the selected tile to -1
		void clearSelection();

		QVBoxLayout* layout = nullptr;
		
		Tristeon::TileSet* tileSet;
		QLabel* image;

		QLabel* tileHighlight = nullptr;
		Tristeon::Tile selectedTile;
	};
}

#endif