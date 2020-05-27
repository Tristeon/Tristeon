#ifdef TRISTEON_EDITOR
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "ActorEditor.h"
#include <QtWidgets>
#include <Editor/Editor.h>
#include "Editor/EditorFields.h"
#include <Registers/BehaviourRegister.h>
#include <Editor/Dynamic/Objects/Behaviours/BehaviourEditor.h>

using Tristeon::Actor;

namespace TristeonEditor
{
	OBJECT_EDITOR_CPP(Tristeon::Actor, ActorEditor)
	
	void ActorEditor::initialize()
	{
		//Display default actor properties first, these don't scroll
		layout->addWidget(new QLabel(QString::fromStdString(actor->serialize()["typeID"].get<std::string>())));
		displayActorProperties();

		//Add scroll area
		QScrollArea* scroll = new QScrollArea(this);
		layout->addWidget(scroll);
		scroll->setAlignment(Qt::AlignTop);
		scroll->setWidgetResizable(true);
		scroll->setFrameShape(QFrame::NoFrame);
		scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

		scrollArea = new QWidget(scroll);
		scrollLayout = new QVBoxLayout(scrollArea);
		scrollLayout->setAlignment(Qt::AlignTop);
		scrollArea->setLayout(scrollLayout);
		layout->addWidget(scrollArea);
		scroll->setWidget(scrollArea);

		//Display manual properties first, then automatic properties, then behaviours.
		//Each of these should be using the scroll area
		displayProperties();
		displayAutoProperties();
		displayBehaviours();
	}

	void ActorEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		actor = (Actor*)current;
	}

	void ActorEditor::displayActorProperties()
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
		posX->setMinimum(std::numeric_limits<int>::lowest());
		posX->setMaximum(std::numeric_limits<int>::max());
		posX->setSingleStep(0);
		posX->setButtonSymbols(QDoubleSpinBox::NoButtons);
		posX->setValue(actor->position.x);
		connect(posX, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=](int value) { actor->position.x = value; });
		posLayout->addWidget(posX);

		auto* posY = new QSpinBox(formWidget);
		posY->setFocusPolicy(Qt::StrongFocus);
		posY->setMinimum(std::numeric_limits<int>::lowest());
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
		scaleX->setMinimum(0);
		scaleX->setMaximum(std::numeric_limits<float>::max());
		scaleX->setSingleStep(0);
		scaleX->setButtonSymbols(QDoubleSpinBox::NoButtons);
		scaleX->setValue(actor->scale.x);
		connect(scaleX, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](double value) { actor->scale.x = (float)value; });
		scaleLayout->addWidget(scaleX);

		auto* scaleY = new QDoubleSpinBox(formWidget);
		scaleY->setFocusPolicy(Qt::StrongFocus);
		scaleY->setMinimum(0);
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
		rot->setMinimum(std::numeric_limits<float>::lowest());
		rot->setMaximum(std::numeric_limits<float>::max());
		rot->setSingleStep(0);
		rot->setButtonSymbols(QDoubleSpinBox::NoButtons);
		rot->setValue(actor->rotation);
		connect(rot, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](double value) { actor->rotation = (float)value; });
		form->addRow(new QLabel("Rotation", formWidget), rot);
	}

	void ActorEditor::displayProperties()
	{
		//Empty
	}

	void ActorEditor::displayAutoProperties()
	{
		//TODO: Both BehaviourEditor and ActorEditor implement this function, generalization would be great
		auto* formWidget = new QWidget(scrollArea);
		scrollLayout->addWidget(formWidget);
		auto* form = new QFormLayout(formWidget);
		formWidget->setLayout(form);

		data = actor->serialize();
		
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			const std::string& key = it.key();

			if (!shouldDisplay(key))
				continue;
			
			switch (it.value().type())
			{
			case detail::value_t::boolean:
			{
				EditorFields::boolField(form, key, data[key], [=](int state)
					{
						data[key] = (bool)((Qt::CheckState)state == Qt::Checked || (Qt::CheckState)state == Qt::PartiallyChecked); actor->deserialize(data);
					});
				break;
			}
			case detail::value_t::string:
			{
				EditorFields::stringField(form, key, data[key], [=](std::string value) { data[key] = value; actor->deserialize(data); });
				break;
			}
			case detail::value_t::object:
			{
				Tristeon::String const type = it.value().contains("typeID") ? it.value()["typeID"] : "";

				bool const isVector2 = type == TRISTEON_TYPENAME(Tristeon::Vector2);
				bool const isVector2Int = type == TRISTEON_TYPENAME(Tristeon::Vector2Int);
				bool const isVector3 = type == TRISTEON_TYPENAME(Tristeon::Vector3);
				bool const isVector4 = type == TRISTEON_TYPENAME(Tristeon::Vector4);

				QWidget* field;
				if (isVector2 || isVector2Int || isVector3 || isVector4)
				{
					field = new QWidget();
					auto* layout = new QHBoxLayout(field);
					layout->setContentsMargins(0, 0, 0, 0);
					field->setLayout(layout);

					if (isVector2 || isVector3 || isVector4)
					{
						auto* x = EditorFields::floatField(field, it.value()["x"], [=](float value) { data[key]["x"] = value; actor->deserialize(data); });
						auto* y = EditorFields::floatField(field, it.value()["y"], [=](float value) { data[key]["y"] = value; actor->deserialize(data); });
						layout->addWidget(x);
						layout->addWidget(y);
					}
					if (isVector3 || isVector4)
					{
						auto* z = EditorFields::floatField(field, it.value()["z"], [=](float value) { data[key]["z"] = value; actor->deserialize(data); });
						layout->addWidget(z);
					}
					if (isVector4)
					{
						auto* w = EditorFields::floatField(field, it.value()["w"], [=](float value) { data[key]["w"] = value; actor->deserialize(data); });
						layout->addWidget(w);
					}

					if (isVector2Int)
					{
						auto* x = EditorFields::intField(field, it.value()["x"], [=](int value) { data[key]["x"] = value; actor->deserialize(data); });
						auto* y = EditorFields::intField(field, it.value()["y"], [=](int value) { data[key]["y"] = value; actor->deserialize(data); });
						layout->addWidget(x);
						layout->addWidget(y);
					}
				}
				else
				{
					//TODO: Support nested objects in behaviour editor
					field = new QLabel("Nested objects aren't supported yet");
				}

				if (field != nullptr)
					form->addRow(new QLabel(QString::fromStdString(key)), field);
				break;
			}
			case detail::value_t::array:
			{
				//TODO: Support arrays in behaviour editor
				EditorFields::labelField(form, key, "Arrays not supported yet");
				break;
			}
			case detail::value_t::number_float:
			{
				EditorFields::floatField(form, key, it.value(), [=](float value) { data[key] = value; actor->deserialize(data); });
				break;
			}
			case detail::value_t::number_integer:
			{
				EditorFields::intField(form, key, it.value(), [=](int value) { data[key] = value; actor->deserialize(data); });
				break;
			}
			case detail::value_t::number_unsigned:
			{
				EditorFields::intField(form, key, it.value(), 0, std::numeric_limits<int>::max(), [=](int value) { data[key] = (unsigned int)value; actor->deserialize(data); });
				break;
			}
			default:
				break;
			}
		}
	}

	void ActorEditor::displayBehaviours()
	{
		for (auto* behaviour : actor->getBehaviours())
			addBehaviour(behaviour);

		QPushButton* addBehaviour = new QPushButton(this);
		addBehaviour->setStyleSheet("background-color: rgb(0, 170, 0);");
		addBehaviour->setText("Add Behaviour");
		layout->addWidget(addBehaviour);
		connect(addBehaviour, &QPushButton::clicked, this, &ActorEditor::addButtonPressed);
	}

	bool ActorEditor::shouldDisplay(Tristeon::String const& propertyName)
	{
		if (propertyName == "position" ||
			propertyName == "typeID" ||
			propertyName == "scale" ||
			propertyName == "rotation" ||
			propertyName == "name" ||
			propertyName == "behaviours")
			return false;
		
		return true;
	}

	void ActorEditor::actorNameChanged(const QString& name)
	{
		actor->name = name.toStdString();
		Editor::instance()->onSelectedActorNameChanged.invoke(name.toStdString());
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
		auto* frame = new QFrame(scrollArea);
		scrollArea->layout()->addWidget(frame);

		frame->setFrameStyle(QFrame::Box | QFrame::Raised);
		frame->setLineWidth(2);
		auto * frameLayout = new QVBoxLayout(frame);
		frame->setLayout(frameLayout);

		json j = behaviour->serialize();

		ObjectEditor * widget = ObjectEditorRegister::createInstance(j["typeID"]);

		if (widget == nullptr)
			widget = new BehaviourEditor(); //Default editor for behaviours as fallback.

		widget->setParent(frame);
		widget->target(behaviour);
		widget->initialize();
		frameLayout->addWidget(widget);
	}
}
#endif