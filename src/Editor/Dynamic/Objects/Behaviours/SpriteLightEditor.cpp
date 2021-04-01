#include "SpriteLightEditor.h"

#include <qformlayout.h>

#include "Settings.h"
#include "Editor/EditorFields.h"

namespace TristeonEditor
{
	void SpriteLightEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		LightEditor::targetChanged(current, old);
		_spriteLight = dynamic_cast<Tristeon::SpriteLight*>(current);
	}

	void SpriteLightEditor::displayContents()
	{
		LightEditor::displayContents();

		auto* formWidget = new QWidget(this);
		layout->addWidget(formWidget);
		auto* form = new QFormLayout(formWidget);
		formWidget->setLayout(form);
		form->setContentsMargins(0, 0, 0, 0);

		EditorFields::vectorUField(form, "Size", { _spriteLight->width(), _spriteLight->height() },
			[=](unsigned int pValX) { _spriteLight->setWidth(pValX); },
			[=](unsigned int pValY) { _spriteLight->setHeight(pValY); }
		);

		EditorFields::vectorBField(form, "Flip", { _spriteLight->flipX(), _spriteLight->flipY() },
			[=](bool pValX) { _spriteLight->setFlipX(pValX); },
			[=](bool pValY) { _spriteLight->setFlipY(pValY); }
		);

		layout->addWidget(new QLabel("Texture"));
		auto* changeAlbedo = new QPushButton(_spriteLight->texturePath().c_str(), this);
		layout->addWidget(changeAlbedo);
		connect(changeAlbedo, &QPushButton::clicked, this, [=]() { changeTexturePressed(changeAlbedo); });
	}
	
	void SpriteLightEditor::changeTexturePressed(QPushButton* pButton)
	{
		QDir const baseDir(Tristeon::Settings::assetPath().c_str());

		auto const path = QFileDialog::getOpenFileName(this, tr("Find Texture"), Tristeon::Settings::assetPath().c_str(), tr("Image Files (*.png *.jpg *.bmp)"));
		auto const localPath = baseDir.relativeFilePath(path);
		auto const fileName = QFileInfo(path).baseName();

		_spriteLight->setTexture(localPath.toStdString(), false);
		pButton->setText(localPath);
	}
}
