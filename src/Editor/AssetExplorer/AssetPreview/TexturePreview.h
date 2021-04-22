#pragma once
#include "AssetPreview.h"
#include <qbrush.h>

namespace TristeonEditor
{
	class TexturePreview : public AssetPreview
	{
	public:
		TexturePreview(const Tristeon::String& path) : AssetPreview(path) { }
		void paintEvent(QPaintEvent* e) override;
	};
}