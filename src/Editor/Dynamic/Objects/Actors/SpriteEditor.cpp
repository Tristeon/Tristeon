#ifdef TRISTEON_EDITOR
#include <magic_enum.hpp>
#include "Editor/Palette.h"
#include "Editor/EditorFields.h"
#include "Settings.h"
#include "SpriteEditor.h"
#include <QtWidgets>

namespace TristeonEditor
{
	void SpriteEditor::targetChanged(Tristeon::TObject* pCurrent, Tristeon::TObject* pOld)
	{
		GraphicEditor::targetChanged(pCurrent, pOld);

		_sprite = (Tristeon::Sprite*)pCurrent;
	}

	void SpriteEditor::displayProperties()
	{
		GraphicEditor::displayProperties();
		
		auto* formWidget = new QWidget(scrollArea);
		auto* form = new QFormLayout(formWidget);
		formWidget->setLayout(form);
		scrollLayout->addWidget(formWidget);

		EditorFields::boolField(form, "Display", _sprite->display, [=](int value) { _sprite->display = value; });
		EditorFields::uintField(form, "Width", _sprite->width, [=](int value) { _sprite->width = value; });
		EditorFields::uintField(form, "Height", _sprite->height, [=](int value) { _sprite->height = value; });

		auto spriteColor = QColor(
			(int)(_sprite->colour.r * 255),
			(int)(_sprite->colour.g * 255),
			(int)(_sprite->colour.b * 255),
			(int)(_sprite->colour.a * 255));
		_colorButton = new QPushButton(formWidget);
		auto const colorStyle = "background-color: rgb(" + std::to_string(spriteColor.red()) + ", " + std::to_string(spriteColor.green()) + ", " + std::to_string(spriteColor.blue()) + ");";
		_colorButton->setStyleSheet(QString::fromStdString(colorStyle));
		form->addRow(new QLabel("Color", formWidget), _colorButton);

		connect(_colorButton, &QPushButton::clicked, this, [&]()
		{
			auto* color = new QColorDialog(this);
			color->setOption(QColorDialog::ShowAlphaChannel, true);
			color->setCurrentColor(spriteColor);
			color->open();
			connect(color, &QColorDialog::currentColorChanged, this, [&](const QColor& color)
			{
				_sprite->colour.r = color.redF();
				_sprite->colour.g = color.greenF();
				_sprite->colour.b = color.blueF();
				_sprite->colour.a = color.alphaF();
				_colorButton->setStyleSheet(Palette::getRGBString("background-color", _sprite->colour));
			});
		});

		EditorFields::boolField(form, "Flip X", _sprite->flipX, [=](bool value) { _sprite->flipX = value; });
		EditorFields::boolField(form, "Flip Y", _sprite->flipY, [=](bool value) { _sprite->flipY = value; });

		displayTextureButtons();
	}

	bool SpriteEditor::shouldDisplay(Tristeon::String const& pPropertyName)
	{
		if (pPropertyName == "display" || pPropertyName == "width" || pPropertyName == "height" || pPropertyName == "colour" || pPropertyName == "texturePath" || pPropertyName == "normalPath" || pPropertyName == "lightMaskPath" || pPropertyName == "flipX" || pPropertyName == "flipY")
			return false;
		return GraphicEditor::shouldDisplay(pPropertyName);
	}

	void SpriteEditor::displayTextureButtons()
	{
		scrollLayout->addWidget(new QLabel("Albedo"));
		auto* changeAlbedo = new QPushButton(_sprite->texturePath().c_str(), this);
		scrollLayout->addWidget(changeAlbedo);
		connect(changeAlbedo, &QPushButton::clicked, this, [=]() { changeTexturePressed(Tristeon::TextureType::Albedo, changeAlbedo); });

		scrollLayout->addWidget(new QLabel("Normal"));
		auto* changeNormal = new QPushButton(_sprite->texturePath(Tristeon::TextureType::Normal).c_str(), this);
		scrollLayout->addWidget(changeNormal);
		connect(changeNormal, &QPushButton::clicked, this, [=]() { changeTexturePressed(Tristeon::TextureType::Normal, changeNormal); });

		scrollLayout->addWidget(new QLabel("Light Mask"));
		auto* changeLightMask = new QPushButton(_sprite->texturePath(Tristeon::TextureType::LightMask).c_str(), this);
		scrollLayout->addWidget(changeLightMask);
		connect(changeLightMask, &QPushButton::clicked, this, [=]() { changeTexturePressed(Tristeon::TextureType::LightMask, changeLightMask); });
	}

	void SpriteEditor::changeTexturePressed(const Tristeon::TextureType& pType, QPushButton* pButton)
	{
		QDir const baseDir(Tristeon::Settings::assetPath().c_str());

		auto const path = QFileDialog::getOpenFileName(this, tr("Find Texture"), Tristeon::Settings::assetPath().c_str(), tr("Image Files (*.png *.jpg *.bmp)"));
		auto const localPath = baseDir.relativeFilePath(path);
		auto const fileName = QFileInfo(path).baseName();

		_sprite->setTexture(localPath.toStdString(), false, pType);
		pButton->setText(localPath);
	}
}
#endif