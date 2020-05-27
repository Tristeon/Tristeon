#ifdef TRISTEON_EDITOR
#include "Animations/AnimationClip.h"
#include "Resources.h"
#include "AnimationClipFileEditor.h"
#include "Editor/EditorFields.h"

namespace TristeonEditor
{
	FILE_EDITOR_CPP("clip", AnimationClipFileEditor)

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

		auto* changeTexture = new QPushButton("Change Texture", this);
		layout->addWidget(changeTexture);
		connect(changeTexture, &QPushButton::clicked, this, [&]()
			{
				QDir const baseDir(QDir::currentPath());

				QString const path = QFileDialog::getOpenFileName(this, tr("Find Texture"), QDir::currentPath() + "/Project", tr("Image Files (*.png *.jpg *.bmp)"));
				QString const localPath = baseDir.relativeFilePath(path);
				QString const fileName = QFileInfo(path).baseName();
				if (path.isEmpty() || localPath.isEmpty())
					return;

				data["texturePath"] = localPath.toStdString();
				saveData();
			});
	}

	void AnimationClipFileEditor::saveData()
	{
		JsonFileEditor::saveData();

		if (Tristeon::Resources::loaded(item->path))
			Tristeon::Resources::assetLoad<Tristeon::AnimationClip>(item->path)->deserialize(data);
	}
}
#endif