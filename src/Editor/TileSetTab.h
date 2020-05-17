#pragma once
#include <QWidget>

#include "TypeDefinitions.h"
#include "FileTypes/TileSet.h"

namespace TristeonEditor
{
	class TileSetTab : public QWidget
	{
	public:
		///<summary>Specify the path of the tileset png. It doesn't allow you to set a parent as tabs that get added to a QTabWidget shouldn't have a parent when constructed as they get added later</summary>
		explicit TileSetTab(const Tristeon::String& tileSetPath);
	private:
		Tristeon::TileSet* tileSet;
	};
}