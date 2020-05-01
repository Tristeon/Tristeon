#ifdef TRISTEON_EDITOR
#include "SpriteEditor.h"
#include <QtWidgets>

namespace TristeonEditor
{
	CUSTOM_EDITOR_CPP(Tristeon::Sprite, SpriteEditor)

	void SpriteEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		ActorEditor::targetChanged(current, old);

		sprite = (Tristeon::Sprite*)current;
	}

	void SpriteEditor::displayProperties()
	{
		ActorEditor::displayProperties();

		auto* formWidget = new QWidget(this);
		auto* form = new QFormLayout(formWidget);
		formWidget->setLayout(form);
		layout->addWidget(formWidget);

		auto* width = new QSpinBox(formWidget);
		width->setMinimum(std::numeric_limits<int>::min());
		width->setMaximum(std::numeric_limits<int>::max());
		width->setSingleStep(0);
		width->setButtonSymbols(QDoubleSpinBox::NoButtons);
		width->setValue(sprite->width);
		connect(width, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=](int value) { sprite->width = value; });
		form->addRow(new QLabel("Width", formWidget), width);

		auto* height = new QSpinBox(formWidget);
		height->setMinimum(std::numeric_limits<int>::min());
		height->setMaximum(std::numeric_limits<int>::max());
		height->setSingleStep(0);
		height->setButtonSymbols(QDoubleSpinBox::NoButtons);
		height->setValue(sprite->height);
		connect(height, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=](int value) { sprite->height = value; });
		form->addRow(new QLabel("Height", formWidget), height);

		QColor spriteColor = QColor(
			(int)(sprite->colour.r * 255),
			(int)(sprite->colour.g * 255),
			(int)(sprite->colour.b * 255),
			(int)(sprite->colour.a * 255));
		colorButton = new QPushButton(formWidget);
		Tristeon::String const colorStyle = "background-color: rgb(" + std::to_string(spriteColor.red()) + ", " + std::to_string(spriteColor.green()) + ", " + std::to_string(spriteColor.blue()) + ");";
		colorButton->setStyleSheet(QString::fromStdString(colorStyle));
		form->addRow(new QLabel("Color", formWidget), colorButton);

		connect(colorButton, &QPushButton::clicked, this, [&]()
		{
			auto* color = new QColorDialog(this);
			color->setCurrentColor(spriteColor);
			color->open();
			connect(color, &QColorDialog::currentColorChanged, this, [&](const QColor& color)
			{
				sprite->colour.r = color.redF();
				sprite->colour.g = color.greenF();
				sprite->colour.b = color.blueF();
				sprite->colour.a = color.alphaF();

				Tristeon::String const newColorStyle = "background-color: rgb(" + std::to_string(color.red()) + ", " + std::to_string(color.green()) + ", " + std::to_string(color.blue()) + ");";
				colorButton->setStyleSheet(QString::fromStdString(newColorStyle));
			});
		});
	}
}
#endif