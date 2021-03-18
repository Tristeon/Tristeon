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
		_light = dynamic_cast<Tristeon::Light*>(current);
	}

	void LightEditor::displayContents()
	{
		auto* formWidget = new QWidget(this);
		layout->addWidget(formWidget);
		auto* form = new QFormLayout(formWidget);
		formWidget->setLayout(form);
		form->setContentsMargins(0, 0, 0, 0);
		
		constexpr const auto& names = magic_enum::enum_names<Tristeon::RenderMask>();
		Tristeon::List<Tristeon::String> options;
		for (auto name : names)
		{
			options.add((Tristeon::String)name);
		}
		options.add("All");

		auto current = magic_enum::enum_index<Tristeon::RenderMask>(_light->renderMask());
		if (_light->renderMask() == Tristeon::RenderMask::All)
			current = 9;
		EditorFields::dropdown(form, "Render Mask Flags", current.has_value() ? (int)current.value() : (int)Tristeon::RenderMask::All, options, [=](int idx)
			{
				_light->setRenderMask(magic_enum::enum_value<Tristeon::RenderMask>(idx));
				if (idx == 9)
					_light->setRenderMask(Tristeon::RenderMask::All);
			});

		EditorFields::header(form, "Properties");
		EditorFields::floatField(form, "Intensity", _light->intensity(), 0, std::numeric_limits<float>::max(), [=](float val) { _light->setIntensity(val); });

		auto color = QColor(
			(int)(_light->colour().r * 255),
			(int)(_light->colour().g * 255),
			(int)(_light->colour().b * 255),
			(int)(_light->colour().a * 255));
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
						_light->setColour({ (float)color.redF(), (float)color.greenF(), (float)color.blueF(), (float)color.alphaF() });
						colorButton->setStyleSheet(Palette::getRGBString("background-color", _light->colour()));
					});
			});
	}
}

#endif