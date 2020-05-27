#include "Editor/EditorFields.h"
#ifdef TRISTEON_EDITOR
#include "SpriteEditor.h"
#include <QtWidgets>

namespace TristeonEditor
{
	OBJECT_EDITOR_CPP(Tristeon::Sprite, SpriteEditor)

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

		EditorFields::boolField(form, "Flip X", sprite->flipX, [=](bool value) { sprite->flipX = value; });
		EditorFields::boolField(form, "Flip Y", sprite->flipY, [=](bool value) { sprite->flipY = value; });
		
		textureButton();
	}

	bool SpriteEditor::shouldDisplay(Tristeon::String const& propertyName)
	{
		if (propertyName == "display" || propertyName == "width" || propertyName == "height" || propertyName == "colour" || propertyName == "texturePath" || propertyName == "flipX" || propertyName == "flipY")
			return false;
		return ActorEditor::shouldDisplay(propertyName);
	}

	void SpriteEditor::textureButton()
	{
		QPushButton* changeTexture = new QPushButton("Change Texture", this);
		scrollLayout->addWidget(changeTexture);
		connect(changeTexture, &QPushButton::clicked, this, &SpriteEditor::changeTexturePressed);
	}

	void SpriteEditor::changeTexturePressed()
	{
		QDir const baseDir(QDir::currentPath());

		QString const path = QFileDialog::getOpenFileName(this, tr("Find Texture"), QDir::currentPath() + "/Project", tr("Image Files (*.png *.jpg *.bmp)"));
		QString const localPath = baseDir.relativeFilePath(path);
		QString const fileName = QFileInfo(path).baseName();
		if (path.isEmpty() || localPath.isEmpty())
			return;

		sprite->setTexture(localPath.toStdString(), false);
	}
}
#endif