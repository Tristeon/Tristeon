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
		form->setContentsMargins(0, 0, 0, 0);
		
		EditorFields::floatField(form, "Radius", circle->radius(), [=](float value) { circle->setRadius(value); });

		auto* offsetWidget = new QWidget();
		auto* offsetLayout = new QHBoxLayout(offsetWidget);
		offsetLayout->setContentsMargins(0, 0, 0, 0);
		offsetWidget->setLayout(offsetLayout);
		QWidget* offsetX = EditorFields::floatField(offsetWidget, circle->offset().x, [=](float value) { circle->setOffset({ value, circle->offset().y }); });
		QWidget* offsetY = EditorFields::floatField(offsetWidget, circle->offset().y, [=](float value) { circle->setOffset({ circle->offset().x, value }); });
		offsetLayout->addWidget(offsetX);
		offsetLayout->addWidget(offsetY);
		form->addRow("Offset", offsetWidget);

		EditorFields::boolField(form, "Is Sensor", circle->sensor(), [=](bool value) { circle->setSensor(value); });
		
		EditorFields::header(form, "Physics properties");
		EditorFields::floatField(form, "Density", circle->density(), [=](float value) { circle->setDensity(value); });
		EditorFields::floatField(form, "Friction", circle->friction(), [=](float value) { circle->setFriction(value); });
		EditorFields::floatField(form, "Restitution", circle->restitution(), [=](float value) { circle->setRestitution(value); });
	}
}
#endif