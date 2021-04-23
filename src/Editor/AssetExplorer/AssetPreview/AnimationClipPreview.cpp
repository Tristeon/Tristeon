#include <Editor/AssetExplorer/AssetPreview/AnimationClipPreview.h>
#include <qpaintdevice.h>
#include <qpainter.h>
#include <qbrush.h>
#include <qtimer.h>

#include <Animations/AnimationClip.h>
#include <AssetManagement/Resources.h>

namespace TristeonEditor
{
	AnimationClipPreview::AnimationClipPreview(const Tristeon::String& path) : AssetPreview(path) 
	{
		QTimer* timer = new QTimer(this);
		QObject::connect(timer, &QTimer::timeout, this, [=]() { repaint(); });
		timer->start(100);
	}
	
	float idx = 0;
	void AnimationClipPreview::paintEvent(QPaintEvent* e)
	{
		auto j = Tristeon::JsonSerializer::load(Tristeon::Settings::assetPath() + _path);
		if (j.empty())
			return;

		Tristeon::AnimationClip clip;
		clip.deserialize(j);

		QPainter painter;
		painter.begin(this);

		const auto brush = QBrush(QImage((Tristeon::Settings::assetPath() + clip.albedoPath).c_str()));

		if (!_animate)
		{
			auto size = brush.textureImage().size();
			size.scale(width(), height(), Qt::KeepAspectRatio);
			
			const auto dst = QRect(0, 0, width(), height());
			const auto src = QRect(0, 0, size.width(), size.height());

			painter.setRenderHint(QPainter::Antialiasing, false);
			painter.drawImage(dst, brush.textureImage(), src, Qt::AutoColor);
			painter.end();
		}
		
		auto size = brush.textureImage().size();
		size.scale(width(), height(), Qt::KeepAspectRatio);

		const int curr = (int)idx + clip.startIndex;
		const int frameX = curr % clip.cols;
		const int frameY = curr / clip.rows;

		const Tristeon::VectorI texSize = { brush.texture().width(), brush.texture().height() };
		
		//Determine the amount of pixels per tile
		const auto framePixelsX = (uint(texSize.x) - ((clip.spacing.left + clip.spacing.right) + ((clip.cols - uint(1)) * clip.spacing.horizontalFrame))) / clip.cols;
		const auto framePixelsY = (uint(texSize.y) - ((clip.spacing.top + clip.spacing.bottom) + ((clip.rows - uint(1)) * clip.spacing.verticalFrame))) / clip.rows;

		//Determine the start point of the tile depending on spacing
		const auto startX = clip.spacing.left + (frameX * framePixelsX) + (frameX * clip.spacing.horizontalFrame);
		const auto startY = clip.spacing.bottom + (frameY * framePixelsY) + (frameY * clip.spacing.verticalFrame);

		const auto dst = QRect(0, 0, size.width(), size.height());
		const auto src = QRect(startX, startY, framePixelsX, framePixelsY);

		painter.setRenderHint(QPainter::Antialiasing, false);
		painter.drawImage(dst, brush.textureImage(), src, Qt::AutoColor);
		painter.end();

		idx += 1 * clip.playbackRate;
		idx = fmod(idx, (clip.endIndex - clip.startIndex + 1));
	}
}