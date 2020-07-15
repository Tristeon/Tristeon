#ifdef TRISTEON_EDITOR
#include "Editor/EditorFields.h"
#include "BoxColliderEditor.h"

namespace TristeonEditor
{
	OBJECT_EDITOR_CPP(Tristeon::BoxCollider, BoxColliderEditor);

	void BoxColliderEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		BehaviourEditor::targetChanged(current, old);
		box = dynamic_cast<Tristeon::BoxCollider*>(current);
	}
	
	void BoxColliderEditor::displayContents()
	{
		auto* formWidget = new QWidget(this);
		layout->addWidget(formWidget);
		auto* form = new QFormLayout(formWidget);
		form->setContentsMargins(0, 0, 0, 0);
		formWidget->setLayout(form);

		auto* sizeWidget = new QWidget();
		auto* sizeLayout = new QHBoxLayout(sizeWidget);
		sizeLayout->setContentsMargins(0, 0, 0, 0);
		sizeWidget->setLayout(sizeLayout);
		QWidget* widthWidget = EditorFields::floatField(sizeWidget, box->width(), [=](float value) { box->width(value); });
		QWidget* heightWidget = EditorFields::floatField(sizeWidget, box->height(), [=](float value) { box->height(value); });
		sizeLayout->addWidget(widthWidget);
		sizeLayout->addWidget(heightWidget);
		form->addRow("Size", sizeWidget);

		auto* offsetWidget = new QWidget();
		auto* offsetLayout = new QHBoxLayout(offsetWidget);
		offsetLayout->setContentsMargins(0, 0, 0, 0);
		offsetWidget->setLayout(offsetLayout);
		QWidget* offsetX = EditorFields::floatField(offsetWidget, box->offset().x, [=](float value) { box->offset({ value, box->offset().y }); });
		QWidget* offsetY = EditorFields::floatField(offsetWidget, box->offset().y, [=](float value) { box->offset({ box->offset().x, value }); });
		offsetLayout->addWidget(offsetX);
		offsetLayout->addWidget(offsetY);
		form->addRow("Offset", offsetWidget);

		EditorFields::floatField(form, "Rotation offset", box->rotationOffset(), [=](float value) { box->rotationOffset(value); });
		EditorFields::boolField(form, "Is Sensor", box->sensor(), [=](bool value) { box->sensor(value); });

		EditorFields::header(form, "Physics properties");
		EditorFields::floatField(form, "Density", box->density(), [=](float value) { box->density(value); });
		EditorFields::floatField(form, "Friction", box->friction(), [=](float value) { box->friction(value); });
		EditorFields::floatField(form, "Restitution", box->restitution(), [=](float value) { box->restitution(value); });
	}
}
#endif