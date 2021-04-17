#include "ColourEditor.h"

#include <qcolordialog.h>
#include <QOpenGLContext>

#include <Editor/Palette.h>

namespace TristeonEditor
{
	ColourEditor::ColourEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback): AbstractJsonEditor(pValue, pCallback)
	{
		const auto colour = pValue.get<Tristeon::Colour>();
		const auto qcolor = QColor(
			(int)(colour.r * 255),
			(int)(colour.g * 255),
			(int)(colour.b * 255),
			(int)(colour.a * 255));
		
		_button = new QPushButton();
		_widget = _button;
		_button->setStyleSheet(Palette::getRGBString("background-color", colour));

		QWidget::connect(_button, &QPushButton::clicked, [=]()
			{
				auto* color = new QColorDialog();
				color->setOption(QColorDialog::ShowAlphaChannel, true);
				color->setCurrentColor(qcolor);
				color->open();

				QWidget::connect(color, &QColorDialog::currentColorChanged, [=](const QColor& color)
					{
						auto newColour = Tristeon::Colour{ (float)color.redF(), (float)color.greenF(), (float)color.blueF(), (float)color.alphaF() };
						_value = newColour;
						_button->setStyleSheet(Palette::getRGBString("background-color", newColour));
						_callback(_value);
					});
			
				QWidget::connect(color, &QColorDialog::rejected, [=]()
					{
						_value = colour;
						_button->setStyleSheet(Palette::getRGBString("background-color", colour));
						_callback(_value);
					});
			});
	}

	void ColourEditor::setValue(const nlohmann::json& pValue)
	{
		_value = pValue;
		_button->setStyleSheet(Palette::getRGBString("background-color", pValue.get<Tristeon::Colour>()));
	}
}