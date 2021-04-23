#pragma once
#include "TexturePreview.h"
#include "AssetPreviewRegister.h"

namespace TristeonEditor
{
	class TilesetPreview : public TexturePreview
	{
		//Empty
	};

	ASSET_PREVIEW_NAME("tileset", TilesetPreview, tileset);
}