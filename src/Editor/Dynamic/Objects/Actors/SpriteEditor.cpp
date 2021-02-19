#ifdef TRISTEON_EDITOR
#include "Editor/Palette.h"
#include "Editor/EditorFields.h"
#include "Settings.h"
#include "SpriteEditor.h"
#include <QtWidgets>

namespace TristeonEditor
{
	void SpriteEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		ActorEditor::targetChanged(current, old);

		sprite = (Tristeon::Sprite*)current;
	}

	void SpriteEditor::displayProperties()
	{
		ActorEditor::displayProperties();

		auto* formWidget = new QWidget(scrollArea);
		auto* form = new QFormLayout(formWidget);
		formWidget->setLayout(form);
		scrollLayout->addWidget(formWidget);

		EditorFields::boolField(form, "Display", sprite->display, [=](int value) { sprite->display = value; });
		EditorFields::uintField(form, "Width", sprite->width, [=](int value) { sprite->width = value; });
		EditorFields::uintField(form, "Height", sprite->height, [=](int value) { sprite->height = value; });

		auto spriteColor = QColor(
			(int)(sprite->colour.r * 255),
			(int)(sprite->colour.g * 255),
			(int)(sprite->colour.b * 255),
			(int)(sprite->colour.a * 255));
		colorButton = new QPushButton(formWidget);
		auto const colorStyle = "background-color: rgb(" + std::to_string(spriteColor.red()) + ", " + std::to_string(spriteColor.green()) + ", " + std::to_string(spriteColor.blue()) + ");";
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
				colorButton->setStyleSheet(Palette::getRGBString("background-color", sprite->colour));
			});
		});

		EditorFields::boolField(form, "Flip X", sprite->flipX, [=](bool value) { sprite->flipX = value; });
		EditorFields::boolField(form, "Flip Y", sprite->flipY, [=](bool value) { sprite->flipY = value; });

		displayTextureButtons();
	}

	bool SpriteEditor::shouldDisplay(Tristeon::String const& propertyName)
	{
		if (propertyName == "display" || propertyName == "width" || propertyName == "height" || propertyName == "colour" || propertyName == "texturePath" || propertyName == "normalPath" || propertyName == "lightMaskPath" || propertyName == "flipX" || propertyName == "flipY")
			return false;
		return ActorEditor::shouldDisplay(propertyName);
	}

	void SpriteEditor::displayTextureButtons()
	{
		scrollLayout->addWidget(new QLabel("Albedo"));
		auto* changeAlbedo = new QPushButton(sprite->texturePath().c_str(), this);
		scrollLayout->addWidget(changeAlbedo);
		connect(changeAlbedo, &QPushButton::clicked, this, [=]() { changeTexturePressed(Tristeon::TextureType::Albedo, changeAlbedo); });

		scrollLayout->addWidget(new QLabel("Normal"));
		auto* changeNormal = new QPushButton(sprite->texturePath(Tristeon::TextureType::Normal).c_str(), this);
		scrollLayout->addWidget(changeNormal);
		connect(changeNormal, &QPushButton::clicked, this, [=]() { changeTexturePressed(Tristeon::TextureType::Normal, changeNormal); });

		scrollLayout->addWidget(new QLabel("Light Mask"));
		auto* changeLightMask = new QPushButton(sprite->texturePath(Tristeon::TextureType::LightMask).c_str(), this);
		scrollLayout->addWidget(changeLightMask);
		connect(changeLightMask, &QPushButton::clicked, this, [=]() { changeTexturePressed(Tristeon::TextureType::LightMask, changeLightMask); });
	}

	void SpriteEditor::changeTexturePressed(const Tristeon::TextureType& type, QPushButton* button)
	{
		QDir const baseDir(Tristeon::Settings::assetPath().c_str());

		auto const path = QFileDialog::getOpenFileName(this, tr("Find Texture"), Tristeon::Settings::assetPath().c_str(), tr("Image Files (*.png *.jpg *.bmp)"));
		auto const localPath = baseDir.relativeFilePath(path);
		auto const fileName = QFileInfo(path).baseName();

		sprite->setTexture(localPath.toStdString(), false, type);
		button->setText(localPath);
	}
}
#endif