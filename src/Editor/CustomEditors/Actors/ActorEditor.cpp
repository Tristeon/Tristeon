#ifdef TRISTEON_EDITOR
#include "ActorEditor.h"
#include <QtWidgets>
#include <Editor/Editor.h>
#include <Editor/CustomEditors/Behaviours/BehaviourEditor.h>

using Tristeon::Actor;

namespace TristeonEditor
{
	CUSTOM_EDITOR_CPP(Tristeon::Actor, ActorEditor)
	
	void ActorEditor::initialize()
	{
		auto* formWidget = new QWidget(this);
		layout->addWidget(formWidget);
		auto* form = new QFormLayout(formWidget);
		formWidget->setLayout(form);

		auto* nameField = new QLineEdit(formWidget);
		nameField->setText(QString::fromStdString(actor->name));
		form->addRow(new QLabel("Name"), nameField);
		connect(nameField, &QLineEdit::textChanged, this, &ActorEditor::actorNameChanged);

		//Position
		auto* posBox = new QWidget(formWidget);
		auto* posLayout = new QHBoxLayout(posBox);
		posLayout->setContentsMargins(0, 0, 0, 0);
		posBox->setLayout(posLayout);

		auto* posX = new QSpinBox(formWidget);
		posX->setMinimum(std::numeric_limits<int>::min());
		posX->setMaximum(std::numeric_limits<int>::max());
		posX->setSingleStep(1);
		posX->setValue(actor->position.x);
		connect(posX, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=](int value) { actor->position.x = value; });
		posLayout->addWidget(posX);
		
		auto* posY = new QSpinBox(formWidget);
		posY->setMinimum(std::numeric_limits<int>::min());
		posY->setMaximum(std::numeric_limits<int>::max());
		posY->setSingleStep(1);
		posY->setValue(actor->position.y);
		connect(posY, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=](int value) { actor->position.y = value; });
		posLayout->addWidget(posY);

		form->addRow(new QLabel("Position", formWidget), posBox);
		
		//Scale
		auto* scaleBox = new QWidget(formWidget);
		auto* scaleLayout = new QHBoxLayout(scaleBox);
		scaleLayout->setContentsMargins(0, 0, 0, 0);
		scaleBox->setLayout(scaleLayout);

		auto* scaleX = new QDoubleSpinBox(formWidget);
		scaleX->setMinimum(std::numeric_limits<float>::min());
		scaleX->setMaximum(std::numeric_limits<float>::max());
		scaleX->setSingleStep(0.1f);
		scaleX->setValue(actor->scale.x);
		connect(scaleX, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](double value) { actor->scale.x = (float)value; });
		scaleLayout->addWidget(scaleX);

		auto* scaleY = new QDoubleSpinBox(formWidget);
		scaleY->setMinimum(std::numeric_limits<float>::min());
		scaleY->setMaximum(std::numeric_limits<float>::max());
		scaleY->setSingleStep(0.1f);
		scaleY->setValue(actor->scale.y);
		connect(scaleY, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](double value) { actor->scale.y = (float)value; });
		scaleLayout->addWidget(scaleY);

		form->addRow(new QLabel("Scale", formWidget), scaleBox);

		//Rotation
		auto* rot = new QDoubleSpinBox(formWidget);
		rot->setMinimum(std::numeric_limits<float>::min());
		rot->setMaximum(std::numeric_limits<float>::max());
		rot->setSingleStep(0.1f);
		rot->setValue(actor->rotation);
		connect(rot, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](double value) { actor->rotation = (float)value; });
		form->addRow(new QLabel("Rotation"), rot);
		
		displayProperties();
		displayBehaviours();
	}

	void ActorEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		actor = (Actor*)current;
	}

	void ActorEditor::displayProperties()
	{
		//Empty
	}

	void ActorEditor::displayBehaviours()
	{
		QScrollArea* area = new QScrollArea(this);
		QVBoxLayout* areaLayout = new QVBoxLayout(area);
		area->setLayout(areaLayout);
		layout->addWidget(area);
		
		for (auto* behaviour : actor->behaviours())
		{
			auto* frame = new QFrame(area);
			areaLayout->addWidget(frame);
			frame->setFrameStyle(QFrame::Box | QFrame::Raised);
			frame->setLineWidth(2);
			auto* frameLayout = new QVBoxLayout(frame);
			frame->setLayout(frameLayout);
			
			json j = behaviour->serialize();

			CustomEditor* widget = EditorRegister::createInstance(j["typeID"]);

			if (widget == nullptr)
				widget = new BehaviourEditor(); //Default editor for behaviours as fallback.

			widget->editor(editor());
			widget->setParent(frame);
			widget->target(behaviour);
			widget->initialize();
			frameLayout->addWidget(widget);
			behaviourWidgets.add(widget);
		}
	}

	void ActorEditor::actorNameChanged(const QString& name)
	{
		actor->name = name.toStdString();
		editor()->onSelectedActorNameChanged.invoke(name.toStdString());
	}
}
#endif