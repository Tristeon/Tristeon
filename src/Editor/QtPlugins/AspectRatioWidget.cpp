#ifdef TRISTEON_EDITOR
#include "AspectRatioWidget.h"
#include <QResizeEvent>

namespace TristeonEditor
{
	/**
	 * Courtesy to https://stackoverflow.com/a/30008442/6896221
	 */
	AspectRatioWidget::AspectRatioWidget(QWidget* widget, float width, float height, QWidget* parent) :
		QWidget(parent), arWidth(width), arHeight(height)
	{
		layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
		layout->setContentsMargins(0, 0, 0, 0);
		
		// add spacer, then your widget, then spacer
		layout->addItem(new QSpacerItem(0, 0));
		layout->addWidget(widget);
		layout->addItem(new QSpacerItem(0, 0));
	}

	void AspectRatioWidget::resizeEvent(QResizeEvent* event)
	{
		//float const thisAspectRatio = (float)event->size().width() / (float)event->size().height();
		//float widgetStretch, outerStretch;

		//if (thisAspectRatio > (arWidth / arHeight)) // too wide
		//{
		//	layout->setDirection(QBoxLayout::LeftToRight);
		//	widgetStretch = (float)height() * (arWidth / arHeight); // i.e., my width
		//	outerStretch = ((float)width() - widgetStretch) / 2.0f + 0.5f;
		//}
		//else // too tall
		//{
		//	layout->setDirection(QBoxLayout::TopToBottom);
		//	widgetStretch = (float)width() * (arHeight / arWidth); // i.e., my height
		//	outerStretch = ((float)height() - widgetStretch) / 2.0f + 0.5f;
		//}

		//layout->setStretch(0, (int)outerStretch);
		//layout->setStretch(1, (int)widgetStretch);
		//layout->setStretch(2, (int)outerStretch);
	}
}
#endif