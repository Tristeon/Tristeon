#pragma once
#include <Editor/AssetExplorer/AssetPreview/AssetPreview.h>
#include <Editor/AssetExplorer/AssetPreview/AssetPreviewRegister.h>

namespace TristeonEditor
{
	class AnimationClipPreview : public AssetPreview
	{
	public:
		AnimationClipPreview(const Tristeon::String& path);
		void paintEvent(QPaintEvent* e) override;
	};

	ASSET_PREVIEW("clip", AnimationClipPreview);
}