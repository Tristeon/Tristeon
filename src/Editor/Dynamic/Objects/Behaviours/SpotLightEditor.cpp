#include "SpotLightEditor.h"

#include <qformlayout.h>

#include "Editor/EditorFields.h"

namespace TristeonEditor
{
	void SpotLightEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		PointLightEditor::targetChanged(current, old);
		_spot = dynamic_cast<Tristeon::SpotLight*>(current);
	}

	void SpotLightEditor::displayContents()
	{
		PointLightEditor::displayContents();
		
		auto* formWidget = new QWidget(this);
		layout->addWidget(formWidget);
		auto* form = new QFormLayout(formWidget);
		formWidget->setLayout(form);
		form->setContentsMargins(0, 0, 0, 0);

		EditorFields::floatField(form, "Inner Cutoff", _spot->innerCutoff(), [=](float pVal) { _spot->setInnerCutoff(pVal); });
		EditorFields::floatField(form, "Outer Cutoff", _spot->outerCutoff(), [=](float pVal) { _spot->setOuterCutoff(pVal); });

		EditorFields::vectorField(form, "Direction", _spot->direction(), 
			[=](float pValX) { _spot->setDirection({ pValX, _spot->direction().y }); },
			[=](float pValY) { _spot->setDirection({ _spot->direction().x, pValY }); }
		);
	}
}
