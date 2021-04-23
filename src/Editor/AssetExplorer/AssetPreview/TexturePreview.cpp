#include "TexturePreview.h"
#include <qpainter.h>
#include <qpaintdevice.h>
#include <qpainterpath.h>

namespace TristeonEditor
{
	void TexturePreview::paintEvent(QPaintEvent* e)
	{
		QPainter painter;
		painter.begin(this);
		const auto brush = QBrush(QImage(_path.c_str()).scaled(width(), height(), Qt::KeepAspectRatio));
		painter.fillRect(0, 0, brush.texture().width(), brush.texture().height(), brush);
		painter.end();
	}
}
