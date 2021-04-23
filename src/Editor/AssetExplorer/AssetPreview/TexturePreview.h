#pragma once
#include "AssetPreview.h"
#include "AssetPreviewRegister.h"
#include <qbrush.h>

namespace TristeonEditor
{
	class TexturePreview : public AssetPreview
	{
	public:
		TexturePreview(const Tristeon::String& path) : AssetPreview(path) { }
		void paintEvent(QPaintEvent* e) override;
	};

	ASSET_PREVIEW_NAME("png", TexturePreview, png);
	ASSET_PREVIEW_NAME("jpg", TexturePreview, jpg);
	ASSET_PREVIEW_NAME("jpeg", TexturePreview, jpeg);
	ASSET_PREVIEW_NAME("tga", TexturePreview, tga);
	ASSET_PREVIEW_NAME("bmp", TexturePreview, bmp);
	ASSET_PREVIEW_NAME("psd", TexturePreview, psd);
}