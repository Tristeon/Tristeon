#ifdef TRISTEON_EDITOR
#include "Animations/AnimationClip.h"
#include "AssetManagement/Resources.h"
#include "AnimationClipFileEditor.h"
#include "Editor/EditorFields.h"

namespace TristeonEditor
{
	void AnimationClipFileEditor::initialize()
	{
		auto* formWidget = new QWidget(this);
		layout->addWidget(formWidget);
		auto* form = new QFormLayout(formWidget);
		formWidget->setLayout(form);
		
		EditorFields::uintField(form, "Columns", data["cols"], [&](uint value) { data["cols"] = value; saveData(); });
		EditorFields::uintField(form, "Rows", data["rows"], [&](uint value) { data["rows"] = value; saveData(); });

		auto* spacing = new QWidget(formWidget);
		layout->addWidget(spacing);
		auto* spacingLayout = new QFormLayout(spacing);
		spacing->setLayout(spacingLayout);
		
		EditorFields::uintField(spacingLayout, "Left", data["spacing"]["left"], [&](uint value) { data["spacing"]["left"] = value; saveData(); });
		EditorFields::uintField(spacingLayout, "Right", data["spacing"]["right"], [&](uint value) { data["spacing"]["right"] = value; saveData(); });
		EditorFields::uintField(spacingLayout, "Top", data["spacing"]["top"], [&](uint value) { data["spacing"]["top"] = value; saveData(); });
		EditorFields::uintField(spacingLayout, "Bottom", data["spacing"]["bottom"], [&](uint value) { data["spacing"]["bottom"] = value; saveData(); });
		EditorFields::uintField(spacingLayout, "Horizontal", data["spacing"]["horizontalFrame"], [&](uint value) { data["spacing"]["horizontalFrame"] = value; saveData(); });
		EditorFields::uintField(spacingLayout, "Vertical", data["spacing"]["verticalFrame"], [&](uint value) { data["spacing"]["verticalFrame"] = value; saveData(); });
		form->addRow(new QLabel("Spacing"), spacing);

		EditorFields::uintField(form, "Start Index", data["startIndex"], [&](uint value) { data["startIndex"] = value; saveData(); });
		EditorFields::uintField(form, "End Index", data["endIndex"], [&](uint value) { data["endIndex"] = value; saveData(); });

		EditorFields::boolField(form, "Loops", data["loops"], [&](int value) { data["loops"] = bool((Qt::CheckState)value != Qt::Unchecked); saveData(); });

		EditorFields::floatField(form, "Playback Rate", data["playbackRate"], [&](float value) { data["playbackRate"] = value; saveData(); });

		layout->addWidget(new QLabel("Albedo"));
		auto* changeAlbedo = new QPushButton(data.value("texturePath", "").c_str(), this);
		layout->addWidget(changeAlbedo);
		connect(changeAlbedo, &QPushButton::clicked, this, [=]() { changeTexturePressed("texturePath", changeAlbedo); });

		layout->addWidget(new QLabel("Normal"));
		auto* changeNormal = new QPushButton(data.value("normalPath", "").c_str(), this);
		layout->addWidget(changeNormal);
		connect(changeNormal, &QPushButton::clicked, this, [=]() { changeTexturePressed("normalPath", changeNormal); });

		EditorFields::floatField(form, "Normal map strength", data.value("normalMapStrength", 1.0f), [&](float value) { data["normalMapStrength"] = value; saveData(); });

		layout->addWidget(new QLabel("Light Mask"));
		auto* changeLightMask = new QPushButton(data.value("lightMaskPath", "").c_str(), this);
		layout->addWidget(changeLightMask);
		connect(changeLightMask, &QPushButton::clicked, this, [=]() { changeTexturePressed("lightMaskPath", changeLightMask); });
	}

	void AnimationClipFileEditor::saveData()
	{
		JsonFileEditor::saveData();

		if (Tristeon::Resources::loaded(item->path))
			Tristeon::Resources::jsonLoad<Tristeon::AnimationClip>(item->path)->deserialize(data);
	}

	void AnimationClipFileEditor::changeTexturePressed(const Tristeon::String& idx, QPushButton* button)
	{
		QDir const baseDir(Tristeon::Settings::assetPath().c_str());

		QString const path = QFileDialog::getOpenFileName(this, tr("Find Texture"), Tristeon::Settings::assetPath().c_str(), tr("Image Files (*.png *.jpg *.bmp)"));
		QString const localPath = baseDir.relativeFilePath(path);
		QString const fileName = QFileInfo(path).baseName();

		data[idx] = localPath.toStdString();
		saveData();
		
		button->setText(localPath);
	}
}
#endif