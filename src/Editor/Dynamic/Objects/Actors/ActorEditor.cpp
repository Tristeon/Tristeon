#ifdef TRISTEON_EDITOR
#include "ActorEditor.h"
#include <QtWidgets>
#include <Editor/Editor.h>
#include <Registers/BehaviourRegister.h>
#include <Editor/Dynamic/Objects/Behaviours/BehaviourEditor.h>

using Tristeon::Actor;

namespace TristeonEditor
{
	OBJECT_EDITOR_CPP(Tristeon::Actor, ActorEditor)
	
	void ActorEditor::initialize()
	{
		displayProperties();
		displayBehaviours();
	}

	void ActorEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		actor = (Actor*)current;
	}

	void ActorEditor::displayProperties()
	{
		auto* formWidget = new QWidget(this);
		layout->addWidget(formWidget);
		auto* form = new QFormLayout(formWidget);
		formWidget->setLayout(form);

		auto* nameField = new QLineEdit(formWidget);
		nameField->setText(QString::fromStdString(actor->name));
		form->addRow(new QLabel("Name", formWidget), nameField);
		connect(nameField, &QLineEdit::textChanged, this, &ActorEditor::actorNameChanged);

		//Position
		auto* posBox = new QWidget(formWidget);
		auto* posLayout = new QHBoxLayout(posBox);
		posLayout->setContentsMargins(0, 0, 0, 0);
		posBox->setLayout(posLayout);

		auto* posX = new QSpinBox(formWidget);
		posX->setFocusPolicy(Qt::StrongFocus);
		posX->setMinimum(std::numeric_limits<int>::min());
		posX->setMaximum(std::numeric_limits<int>::max());
		posX->setSingleStep(0);
		posX->setButtonSymbols(QDoubleSpinBox::NoButtons);
		posX->setValue(actor->position.x);
		connect(posX, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=](int value) { actor->position.x = value; });
		posLayout->addWidget(posX);

		auto* posY = new QSpinBox(formWidget);
		posY->setFocusPolicy(Qt::StrongFocus);
		posY->setMinimum(std::numeric_limits<int>::min());
		posY->setMaximum(std::numeric_limits<int>::max());
		posY->setSingleStep(0);
		posY->setButtonSymbols(QDoubleSpinBox::NoButtons);
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
		scaleX->setFocusPolicy(Qt::StrongFocus);
		scaleX->setMinimum(std::numeric_limits<float>::min());
		scaleX->setMaximum(std::numeric_limits<float>::max());
		scaleX->setSingleStep(0);
		scaleX->setButtonSymbols(QDoubleSpinBox::NoButtons);
		scaleX->setValue(actor->scale.x);
		connect(scaleX, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](double value) { actor->scale.x = (float)value; });
		scaleLayout->addWidget(scaleX);

		auto* scaleY = new QDoubleSpinBox(formWidget);
		scaleY->setFocusPolicy(Qt::StrongFocus);
		scaleY->setMinimum(std::numeric_limits<float>::min());
		scaleY->setMaximum(std::numeric_limits<float>::max());
		scaleY->setSingleStep(0);
		scaleY->setButtonSymbols(QDoubleSpinBox::NoButtons);
		scaleY->setValue(actor->scale.y);
		connect(scaleY, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](double value) { actor->scale.y = (float)value; });
		scaleLayout->addWidget(scaleY);

		form->addRow(new QLabel("Scale", formWidget), scaleBox);

		//Rotation
		auto* rot = new QDoubleSpinBox(formWidget);
		rot->setFocusPolicy(Qt::StrongFocus);
		rot->setMinimum(std::numeric_limits<float>::min());
		rot->setMaximum(std::numeric_limits<float>::max());
		rot->setSingleStep(0);
		rot->setButtonSymbols(QDoubleSpinBox::NoButtons);
		rot->setValue(actor->rotation);
		connect(rot, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](double value) { actor->rotation = (float)value; });
		form->addRow(new QLabel("Rotation", formWidget), rot);
	}

	void ActorEditor::displayBehaviours()
	{
		QScrollArea* scroll = new QScrollArea(this);
		layout->addWidget(scroll);
		scroll->setAlignment(Qt::AlignCenter);
		scroll->setWidgetResizable(true);
		
		behavioursArea = new QWidget(scroll);
		auto* areaLayout = new QVBoxLayout(behavioursArea);
		behavioursArea->setLayout(areaLayout);
		layout->addWidget(behavioursArea);

		for (auto* behaviour : actor->behaviours())
			addBehaviour(behaviour);

		scroll->setWidget(behavioursArea);

		QPushButton* addBehaviour = new QPushButton(this);
		addBehaviour->setStyleSheet("background-color: rgb(0, 170, 0);");
		addBehaviour->setText("Add Behaviour");
		layout->addWidget(addBehaviour);
		connect(addBehaviour, &QPushButton::clicked, this, &ActorEditor::addButtonPressed);
	}

	void ActorEditor::actorNameChanged(const QString& name)
	{
		actor->name = name.toStdString();
		editor()->onSelectedActorNameChanged.invoke(name.toStdString());
	}

	void ActorEditor::addButtonPressed()
	{
		QMenu contextMenu(tr("Context menu"));

		auto* map = Tristeon::BehaviourRegister::getMap();
		for (auto const& pair : *map)
		{
			QAction* action = new QAction(pair.first.c_str(), this);
			connect(action, &QAction::triggered, this, [&](bool checked)
			{
				auto* behaviour = actor->addBehaviour(pair.first);
				addBehaviour(behaviour);
			});
			contextMenu.addAction(action);
		}

		contextMenu.exec(QCursor::pos());
	}

	void ActorEditor::addBehaviour(Tristeon::Behaviour* behaviour)
	{
		auto* frame = new QFrame(behavioursArea);
		behavioursArea->layout()->addWidget(frame);

		frame->setFrameStyle(QFrame::Box | QFrame::Raised);
		frame->setLineWidth(2);
		auto * frameLayout = new QVBoxLayout(frame);
		frame->setLayout(frameLayout);

		json j = behaviour->serialize();

		ObjectEditor * widget = ObjectEditorRegister::createInstance(j["typeID"]);

		if (widget == nullptr)
			widget = new BehaviourEditor(); //Default editor for behaviours as fallback.

		widget->editor(editor());
		widget->setParent(frame);
		widget->target(behaviour);
		widget->initialize();
		frameLayout->addWidget(widget);
	}
}
#endif