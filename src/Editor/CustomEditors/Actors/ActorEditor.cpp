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