#ifdef TRISTEON_EDITOR
#include "Editor/EditorFields.h"

#include "PointLightEditor.h"
#include <qformlayout.h>

namespace TristeonEditor
{
	void PointLightEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		LightEditor::targetChanged(current, old);
		_point = dynamic_cast<Tristeon::PointLight*>(current);
	}

	void PointLightEditor::displayContents()
	{
		LightEditor::displayContents();

		auto* formWidget = new QWidget(this);
		layout->addWidget(formWidget);
		auto* form = new QFormLayout(formWidget);
		formWidget->setLayout(form);
		form->setContentsMargins(0, 0, 0, 0);

		EditorFields::floatField(form, "Z Distance", _point->distance(), [=](float pVal) { _point->setDistance(pVal); });

		EditorFields::floatField(form, "Inner Radius", _point->innerRadius(), [=](float pVal) { _point->setInnerRadius(pVal); });
		EditorFields::floatField(form, "Outer Radius", _point->outerRadius(), [=](float pVal) { _point->setOuterRadius(pVal); });
	}
}
#endif