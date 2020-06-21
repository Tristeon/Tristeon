#ifdef TRISTEON_EDITOR
#include "Editor/EditorFields.h"
#include "CircleColliderEditor.h"

namespace TristeonEditor
{
	OBJECT_EDITOR_CPP(Tristeon::CircleCollider, CircleColliderEditor);

	void CircleColliderEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		BehaviourEditor::targetChanged(current, old);
		circle = dynamic_cast<Tristeon::CircleCollider*>(current);
	}

	void CircleColliderEditor::displayContents()
	{
		auto* formWidget = new QWidget(this);
		layout->addWidget(formWidget);
		auto* form = new QFormLayout(formWidget);
		formWidget->setLayout(form);
		
		EditorFields::floatField(form, "Radius", circle->radius(), [=](float value) { circle->radius(value); });

		auto* offsetWidget = new QWidget();
		auto* offsetLayout = new QHBoxLayout(offsetWidget);
		offsetLayout->setContentsMargins(0, 0, 0, 0);
		offsetWidget->setLayout(offsetLayout);
		QWidget* offsetX = EditorFields::floatField(offsetWidget, circle->offset().x, [=](float value) { circle->offset({ value, circle->offset().y }); });
		QWidget* offsetY = EditorFields::floatField(offsetWidget, circle->offset().y, [=](float value) { circle->offset({ circle->offset().x, value }); });
		offsetLayout->addWidget(offsetX);
		offsetLayout->addWidget(offsetY);
		form->addRow("Offset", offsetWidget);

		EditorFields::boolField(form, "Is Sensor", circle->sensor(), [=](bool value) { circle->sensor(value); });
		
		EditorFields::header(form, "Physics properties");
		EditorFields::floatField(form, "Density", circle->density(), [=](float value) { circle->density(value); });
		EditorFields::floatField(form, "Friction", circle->friction(), [=](float value) { circle->friction(value); });
		EditorFields::floatField(form, "Restitution", circle->restitution(), [=](float value) { circle->restitution(value); });
	}
}
#endif