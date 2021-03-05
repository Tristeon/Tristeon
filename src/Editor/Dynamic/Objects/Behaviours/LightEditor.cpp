#ifdef TRISTEON_EDITOR
#include "LightEditor.h"
#include <Editor/EditorFields.h>
#include <qwidget.h>
#include <magic_enum.hpp>
#include "Editor/Palette.h"

namespace TristeonEditor
{
	void LightEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		BehaviourEditor::targetChanged(current, old);
		light = dynamic_cast<Tristeon::Light*>(current);
	}

	void LightEditor::displayContents()
	{
		auto* formWidget = new QWidget(this);
		layout->addWidget(formWidget);
		auto* form = new QFormLayout(formWidget);
		formWidget->setLayout(form);
		form->setContentsMargins(0, 0, 0, 0);
		
		Tristeon::List<Tristeon::String> options;
		for (auto i = 0; i < static_cast<int>(Tristeon::Light::SourceType::EndOfEnum); i++)
			options.add(magic_enum::enum_name((Tristeon::Light::SourceType)i).data());
		EditorFields::dropdown(form, "Type", (int)light->type(), options, [=](int idx) { light->setType((Tristeon::Light::SourceType)idx); });

		EditorFields::header(form, "Properties");
		EditorFields::floatField(form, "Intensity", light->intensity(), 0, std::numeric_limits<float>::max(), [=](float val) { light->setIntensity(val); });

		EditorFields::floatField(form, "Inner Radius", light->innerRadius(), 0, std::numeric_limits<float>::max(), [=](float val) { light->setInnerRadius(val); });
		EditorFields::floatField(form, "Outer Radius", light->outerRadius(), 0, std::numeric_limits<float>::max(), [=](float val) { light->setOuterRadius(val); });

		EditorFields::floatField(form, "Distance", light->distance(), 0, std::numeric_limits<float>::max(), [=](float val) { light->setDistance(val); });

		auto color = QColor(
			(int)(light->colour().r * 255),
			(int)(light->colour().g * 255),
			(int)(light->colour().b * 255),
			(int)(light->colour().a * 255));
		auto* colorButton = new QPushButton(formWidget);
		auto const colorStyle = "background-color: rgb(" + std::to_string(color.red()) + ", " + std::to_string(color.green()) + ", " + std::to_string(color.blue()) + ");";
		colorButton->setStyleSheet(QString::fromStdString(colorStyle));
		form->addRow(new QLabel("Color", formWidget), colorButton);

		connect(colorButton, &QPushButton::clicked, this, [=]()
			{
				auto* c = new QColorDialog(this);
				c->setCurrentColor(color);
				c->open();
				connect(c, &QColorDialog::currentColorChanged, this, [&](const QColor& color)
					{
						light->setColour({ (float)color.redF(), (float)color.greenF(), (float)color.blueF(), (float)color.alphaF() });
						colorButton->setStyleSheet(Palette::getRGBString("background-color", light->colour()));
					});
			});
		
		EditorFields::header(form, "Spot lights");
		EditorFields::vectorField(form, "Direction", light->direction(), 
			[=](float x) { light->setDirection({ x, light->direction().y }); }, 
			[=](float y) { light->setDirection({ light->direction().x, y }); });
		EditorFields::floatField(form, "Inner Cutoff", light->innerCutoff(), [=](float val) { light->setInnerCutoff(val); });
		EditorFields::floatField(form, "Outer Cutoff", light->outerCutoff(), [=](float val) { light->setOuterCutoff(val); });
	}
}

#endif