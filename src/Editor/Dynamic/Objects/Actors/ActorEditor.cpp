#include "ActorEditor.h"

#include <QtWidgets>
#include <qlayout.h>

#include <Editor/Editor.h>

#include <Serialization/Register.h>

namespace TristeonEditor
{
	ActorEditor::ActorEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback) : AbstractJsonEditor(pValue, pCallback)
	{
		_widget = new QWidget();
		_widget->setLayout(_layout);
		_layout = new QBoxLayout(QBoxLayout::TopToBottom);
		_layout->setContentsMargins(0, 0, 0, 0);

		_layout->addWidget(new QLabel(QString::fromStdString(pValue["typeID"].get<std::string>())));

		_formWidget = new QWidget();
		_formLayout = new QFormLayout();
		_formLayout->setContentsMargins(0, 0, 0, 0);
		_formWidget->setLayout(_formLayout);
		_layout->addWidget(_formWidget);

		_editors["name"] = EditorRegister::createInstance(Tristeon::Type<std::string>::fullName(), pValue["name"], 
			[=](nlohmann::json val) { 
				_value["name"] = val;
				_callback(_value);
				Editor::instance()->onSelectedActorNameChanged.invoke(val.get<std::string>());
			}
		);
		_formLayout->addRow("Name", _editors["name"]->widget());

		_editors["position"] = EditorRegister::createInstance(Tristeon::Type<Tristeon::Vector>::fullName(), pValue["position"],
			[=](nlohmann::json val) {
				_value["position"] = val;
				_callback(_value);
			}
		);
		_formLayout->addRow("position", _editors["position"]->widget());
		
		_editors["scale"] = EditorRegister::createInstance(Tristeon::Type<Tristeon::Vector>::fullName(), pValue["scale"],
			[=](nlohmann::json val) {
				_value["scale"] = val;
				_callback(_value);
			}
		);
		_formLayout->addRow("Scale", _editors["scale"]->widget());

		_editors["rotation"] = EditorRegister::createInstance(Tristeon::Type<float>::fullName(), pValue["rotation"],
			[=](nlohmann::json val) {
				_value["rotation"] = val;
				_callback(_value);
			}
		);
		_formLayout->addRow("Rotation", _editors["rotation"]->widget());
	}

	ActorEditor::~ActorEditor()
	{
		delete _layout;
		delete _widget;
	}

	void ActorEditor::setValue(const nlohmann::json& pValue)
	{
		_value = pValue;
		//TODO
	}

	QWidget* ActorEditor::widget()
	{
		return _widget;
	}

	//void ActorEditor::initialize()
	//{
	//	//Add scroll area
	//	QScrollArea* scroll = new QScrollArea(this);
	//	layout->addWidget(scroll);
	//	scroll->setAlignment(Qt::AlignTop);
	//	scroll->setWidgetResizable(true);
	//	scroll->setFrameShape(QFrame::NoFrame);
	//	scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//	scroll->setContentsMargins(0, 0, 0, 0);

	//	scrollArea = new QWidget(scroll);
	//	scrollLayout = new QVBoxLayout(scrollArea);
	//	scrollLayout->setAlignment(Qt::AlignTop);
	//	scrollLayout->setContentsMargins(0, 0, 0, 0);
	//	scrollArea->setLayout(scrollLayout);
	//	layout->addWidget(scrollArea);
	//	scroll->setWidget(scrollArea);

	//	//Display manual properties first, then automatic properties, then behaviours.
	//	//Each of these should be using the scroll area
	//	displayProperties();
	//	displayAutoProperties();
	//	displayBehaviours();
	//}
	//
	//void ActorEditor::displayProperties()
	//{
	//	//Empty
	//}

	//void ActorEditor::displayAutoProperties()
	//{
	//	//TODO: Both BehaviourEditor and ActorEditor implement this function, generalization would be great
	//	auto* formWidget = new QWidget(scrollArea);
	//	scrollLayout->addWidget(formWidget);
	//	auto* form = new QFormLayout(formWidget);
	//	form->setContentsMargins(0, 0, 0, 0);
	//	formWidget->setLayout(form);

	//	data = actor->serialize();
	//	
	//	for (auto it = data.begin(); it != data.end(); ++it)
	//	{
	//		const std::string& key = it.key();

	//		if (!shouldDisplay(key))
	//			continue;
	//		
	//		switch (it.value().type())
	//		{
	//		case detail::value_t::boolean:
	//		{
	//			EditorFields::boolField(form, key, data[key], [=](int state)
	//				{
	//					data[key] = (bool)((Qt::CheckState)state == Qt::Checked || (Qt::CheckState)state == Qt::PartiallyChecked); actor->deserialize(data);
	//				});
	//			break;
	//		}
	//		case detail::value_t::string:
	//		{
	//			EditorFields::stringField(form, key, data[key], [=](std::string value) { data[key] = value; actor->deserialize(data); });
	//			break;
	//		}
	//		case detail::value_t::object:
	//		{
	//			Tristeon::String const type = it.value().contains("typeID") ? it.value()["typeID"] : "";

	//			bool const isVector = type == Tristeon::Type<Tristeon::Vector>::fullName();
	//			bool const isVectorI = type == Tristeon::Type<Tristeon::VectorI>::fullName();

	//			QWidget* field;
	//			if (isVector || isVectorI)
	//			{
	//				field = new QWidget();
	//				auto* layout = new QHBoxLayout(field);
	//				layout->setContentsMargins(0, 0, 0, 0);
	//				field->setLayout(layout);

	//				if (isVector)
	//				{
	//					auto* x = EditorFields::floatField(field, it.value()["x"], [=](float value) { data[key]["x"] = value; actor->deserialize(data); });
	//					auto* y = EditorFields::floatField(field, it.value()["y"], [=](float value) { data[key]["y"] = value; actor->deserialize(data); });
	//					layout->addWidget(x);
	//					layout->addWidget(y);
	//				}

	//				if (isVectorI)
	//				{
	//					auto* x = EditorFields::intField(field, it.value()["x"], [=](int value) { data[key]["x"] = value; actor->deserialize(data); });
	//					auto* y = EditorFields::intField(field, it.value()["y"], [=](int value) { data[key]["y"] = value; actor->deserialize(data); });
	//					layout->addWidget(x);
	//					layout->addWidget(y);
	//				}
	//			}
	//			else
	//			{
	//				//TODO: Support nested objects in behaviour editor
	//				field = new QLabel("Nested objects aren't supported yet");
	//			}

	//			if (field != nullptr)
	//				form->addRow(new QLabel(QString::fromStdString(key)), field);
	//			break;
	//		}
	//		case detail::value_t::array:
	//		{
	//			//TODO: Support arrays in behaviour editor
	//			EditorFields::labelField(form, key, "Arrays not supported yet");
	//			break;
	//		}
	//		case detail::value_t::number_float:
	//		{
	//			EditorFields::floatField(form, key, it.value(), [=](float value) { data[key] = value; actor->deserialize(data); });
	//			break;
	//		}
	//		case detail::value_t::number_integer:
	//		{
	//			EditorFields::intField(form, key, it.value(), [=](int value) { data[key] = value; actor->deserialize(data); });
	//			break;
	//		}
	//		case detail::value_t::number_unsigned:
	//		{
	//			EditorFields::intField(form, key, it.value(), 0, std::numeric_limits<int>::max(), [=](int value) { data[key] = (unsigned int)value; actor->deserialize(data); });
	//			break;
	//		}
	//		default:
	//			break;
	//		}
	//	}
	//}

	//void ActorEditor::displayBehaviours()
	//{
	//	for (auto* behaviour : actor->findBehaviours())
	//		addBehaviour(behaviour);

	//	QPushButton* addBehaviour = new QPushButton(this);
	//	addBehaviour->setStyleSheet("background-color: rgb(46, 204, 113);");
	//	addBehaviour->setText("Add Behaviour");
	//	layout->addWidget(addBehaviour);
	//	connect(addBehaviour, &QPushButton::clicked, this, &ActorEditor::addButtonPressed);
	//}

	//bool ActorEditor::shouldDisplay(Tristeon::String const& propertyName)
	//{
	//	if (propertyName == "position" ||
	//		propertyName == "typeID" ||
	//		propertyName == "scale" ||
	//		propertyName == "rotation" ||
	//		propertyName == "name" ||
	//		propertyName == "instanceID")
	//		return false;
	//	
	//	return true;
	//}

	//void ActorEditor::addButtonPressed()
	//{
	//	QMenu contextMenu(tr("Context menu"));

	//	auto* map = Tristeon::Register<Tristeon::Behaviour>::getMap();
	//	for (auto const& pair : *map)
	//	{
	//		QAction* action = new QAction(pair.first.c_str(), this);
	//		connect(action, &QAction::triggered, this, [&](bool checked)
	//		{
	//			auto* behaviour = actor->createBehaviour(pair.first);
	//			addBehaviour(behaviour);
	//		});
	//		contextMenu.addAction(action);
	//	}

	//	contextMenu.exec(QCursor::pos());
	//}

	//void ActorEditor::addBehaviour(Tristeon::Behaviour* behaviour)
	//{
	//	json j = behaviour->serialize();

	//	ObjectEditor * widget = ObjectEditorRegister::createInstance(j["typeID"]);

	//	if (widget == nullptr)
	//		widget = new BehaviourEditor(); //Default editor for behaviours as fallback.

	//	widget->setParent(scrollArea);
	//	widget->target(behaviour);
	//	widget->initialize();
	//	scrollArea->layout()->addWidget(widget);
	//}
}