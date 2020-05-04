#ifdef TRISTEON_EDITOR
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
		
		QSpinBox* cols = EditorFields::intField(formWidget, data["cols"].get<int>(), 0, std::numeric_limits<int>::max(), [&](int value)
			{
				data["cols"] = (unsigned int)value;
				saveData();
			});
		form->addRow(new QLabel("Columns"), cols);

		QSpinBox* rows = EditorFields::intField(formWidget, data["rows"].get<int>(), 0, std::numeric_limits<int>::max(), [&](int value)
			{
				data["rows"] = (unsigned int)value;
				saveData();
			});
		form->addRow(new QLabel("Rows"), rows);

		QWidget* spacing = new QWidget(formWidget);
		QFormLayout* spacingLayout = new QFormLayout(spacing);
		spacing->setLayout(spacingLayout);

		QSpinBox* spacingLeft = EditorFields::intField(formWidget, data["spacing"]["left"].get<int>(), 0, std::numeric_limits<int>::max(), [&](int value)
			{
				data["spacing"]["left"] = (unsigned int)value;
				saveData();
			});
		spacingLayout->addRow(new QLabel("Left"), spacingLeft);
		QSpinBox* spacingRight = EditorFields::intField(formWidget, data["spacing"]["right"].get<int>(), 0, std::numeric_limits<int>::max(), [&](int value)
			{
				data["spacing"]["right"] = (unsigned int)value;
				saveData();
			});
		spacingLayout->addRow(new QLabel("Right"), spacingRight);
		QSpinBox* spacingTop = EditorFields::intField(formWidget, data["spacing"]["top"].get<int>(), 0, std::numeric_limits<int>::max(), [&](int value)
			{
				data["spacing"]["top"] = (unsigned int)value;
				saveData();
			});
		spacingLayout->addRow(new QLabel("Top"), spacingTop);
		QSpinBox* spacingBottom = EditorFields::intField(formWidget, data["spacing"]["bottom"].get<int>(), 0, std::numeric_limits<int>::max(), [&](int value)
			{
				data["spacing"]["bottom"] = (unsigned int)value;
				saveData();
			});
		spacingLayout->addRow(new QLabel("Bottom"), spacingBottom);
		QSpinBox* spacingHorizontalFrame = EditorFields::intField(formWidget, data["spacing"]["horizontalFrame"].get<int>(), 0, std::numeric_limits<int>::max(), [&](int value)
			{
				data["spacing"]["horizontalFrame"] = (unsigned int)value;
				saveData();
			});
		spacingLayout->addRow(new QLabel("Bottom"), spacingHorizontalFrame);
		QSpinBox* spacingVerticalFrame = EditorFields::intField(formWidget, data["spacing"]["verticalFrame"].get<int>(), 0, std::numeric_limits<int>::max(), [&](int value)
			{
				data["spacing"]["verticalFrame"] = (unsigned int)value;
				saveData();
			});
		spacingLayout->addRow(new QLabel("Bottom"), spacingVerticalFrame);
		
		form->addRow(new QLabel("Spacing"), spacing);

		QSpinBox* start = EditorFields::intField(formWidget, data["startIndex"].get<int>(), 0, std::numeric_limits<int>::max(), [&](int value)
			{
				data["startIndex"] = (unsigned int)value;
				saveData();
			});
		form->addRow(new QLabel("Start Index"), start);

		QSpinBox* end = EditorFields::intField(formWidget, data["endIndex"].get<int>(), 0, std::numeric_limits<int>::max(), [&](int value)
			{
				data["endIndex"] = (unsigned int)value;
				saveData();
			});
		form->addRow(new QLabel("End Index"), end);

		auto* loops = new QCheckBox(formWidget);
		loops->setCheckState(data["loops"].get<bool>() ? Qt::Checked : Qt::Unchecked);
		connect(loops, &QCheckBox::stateChanged, this, [&](int state)
			{
				data["loops"] = bool((Qt::CheckState)state == Qt::Checked);
				saveData();
			});
		form->addRow(new QLabel("Loops"), loops);

		QDoubleSpinBox* playbackRate = EditorFields::floatField(formWidget, data["playbackRate"].get<float>(), [&](float value)
			{
				data["playbackRate"] = value;
				saveData();
			});
		form->addRow(new QLabel("Playback Rate"), playbackRate);

		QPushButton* changeTexture = new QPushButton("Change Texture", this);
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
}
#endif