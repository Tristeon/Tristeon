#pragma once
#ifdef TRISTEON_EDITOR
#include <QLayout>
#include <QWidget>

namespace TristeonEditor
{
	/**
	 * Courtesy to https://stackoverflow.com/a/30008442/6896221
	 */
	class AspectRatioWidget : public QWidget
	{
	public:
		AspectRatioWidget(QWidget* widget, float width, float height, QWidget* parent = nullptr);
		void resizeEvent(QResizeEvent* event);

	private:
		QBoxLayout* layout;
		float arWidth; // aspect ratio width
		float arHeight; // aspect ratio height
	};
}
#endif