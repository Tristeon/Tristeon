#pragma once
#include <QWidget>

#include "TypeDefinitions.h"
#include "FileTypes/TileSet.h"

namespace TristeonEditor
{
	class TileSetTab : public QWidget
	{
	public:
		explicit TileSetTab(QWidget* parent, const Tristeon::String& path);
	private:
		Tristeon::TileSet* tileSet;
	};
}