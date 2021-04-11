#include "ActorEditor.h"

#include <QtWidgets>
#include <qlayout.h>

#include <Editor/Editor.h>

#include <Serialization/Register.h>


#include "InstanceCollector.h"
#include "Editor/Dynamic/Objects/Behaviours/BehaviourEditor.h"

namespace TristeonEditor
{
	ActorEditor::ActorEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback) : AbstractJsonEditor(pValue, pCallback)
	{
		_widget = new QWidget();
		_layout = new QBoxLayout(QBoxLayout::TopToBottom);
		_layout->setAlignment(Qt::AlignTop);
		_widget->setLayout(_layout);
		_layout->setContentsMargins(0, 0, 0, 0);

		_layout->addWidget(new QLabel(QString::fromStdString(pValue["typeID"].get<std::string>())));

		_formWidget = new QWidget();
		_formLayout = new QFormLayout();
		_formLayout->setContentsMargins(0, 0, 0, 0);
		_formWidget->setLayout(_formLayout);
		_layout->addWidget(_formWidget);

		displayDefaultProperties();

		auto* scroll = new QScrollArea();
		_layout->addWidget(scroll);
		scroll->setAlignment(Qt::AlignTop);
		scroll->setWidgetResizable(true);
		scroll->setFrameShape(QFrame::NoFrame);
		scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		scroll->setContentsMargins(0, 0, 0, 0);

		scrollArea = new QWidget();
		scrollLayout = new QVBoxLayout(scrollArea);
		scrollLayout->setAlignment(Qt::AlignTop);
		scrollLayout->setContentsMargins(0, 0, 0, 0);
		scrollArea->setLayout(scrollLayout);
		_layout->addWidget(scrollArea);
		scroll->setWidget(scrollArea);

		//Display manual properties first, then automatic properties, then behaviours.
		//Each of these should be using the scroll area
		displayAutoProperties();
		displayBehaviours();
	}

	ActorEditor::~ActorEditor()
	{
		delete _layout;
	}

	void ActorEditor::setValue(const nlohmann::json& pValue)
	{
		_value = pValue;
		//TODO
	}

	void ActorEditor::displayDefaultProperties()
	{
		_editors["name"] = EditorRegister::createInstance(Tristeon::Type<std::string>::fullName(), _value["name"],
			[=](nlohmann::json val) {
				_value["name"] = val;
				_callback(_value);
				Editor::instance()->onSelectedActorNameChanged.invoke(val.get<std::string>());
			}
		);
		_formLayout->addRow("Name", _editors["name"]->widget());

		_editors["position"] = EditorRegister::createInstance(Tristeon::Type<Tristeon::Vector>::fullName(), _value["position"],
			[=](nlohmann::json val) {
				_value["position"] = val;
				_callback(_value);
			}
		);
		_formLayout->addRow("Position", _editors["position"]->widget());

		_editors["scale"] = EditorRegister::createInstance(Tristeon::Type<Tristeon::Vector>::fullName(), _value["scale"],
			[=](nlohmann::json val) {
				_value["scale"] = val;
				_callback(_value);
			}
		);
		_formLayout->addRow("Scale", _editors["scale"]->widget());

		_editors["rotation"] = EditorRegister::createInstance(Tristeon::Type<float>::fullName(), _value["rotation"],
			[=](nlohmann::json val) {
				_value["rotation"] = val;
				_callback(_value);
			}
		);
		_formLayout->addRow("Rotation", _editors["rotation"]->widget());
	}

	void ActorEditor::displayAutoProperties()
	{
		auto* formWidget = new QWidget(scrollArea);
		scrollLayout->addWidget(formWidget);
		auto* form = new QFormLayout(formWidget);
		form->setContentsMargins(0, 0, 0, 0);
		formWidget->setLayout(form);

		for (auto it = _value.begin(); it != _value.end(); ++it)
		{
			const std::string& key = it.key();

			if (key == "typeID" || key == "instanceID" || key == "position" || key == "scale" || key == "rotation" || key == "name" || key == "behaviours")
				continue;

			Tristeon::String type;

			switch (it.value().type())
			{
			case detail::value_t::object:
				type = it.value().value("typeID", "");
				break;
			case detail::value_t::array:
				type = "";
				break;
			case detail::value_t::string:
				type = Tristeon::Type<Tristeon::String>::fullName();
				break;
			case detail::value_t::boolean:
				type = Tristeon::Type<bool>::fullName();
				break;
			case detail::value_t::number_integer:
				type = Tristeon::Type<int>::fullName();
				break;
			case detail::value_t::number_unsigned:
				type = Tristeon::Type<unsigned>::fullName();
				break;
			case detail::value_t::number_float:
				type = Tristeon::Type<float>::fullName();
				break;
			default: break;
			}

			if (type.empty())
				form->addRow(key.c_str(), new QLabel("Type not supported"));
			else
			{
				_editors[key] = EditorRegister::createInstance(type, _value[key],
					[=](nlohmann::json val) {
						_value[key] = val;
						_callback(_value);
					}
				);
				if (_editors[key])
					form->addRow(key.c_str(), _editors[key]->widget());
				else
					form->addRow(key.c_str(), new QLabel("No custom editor found"));
			}
		}

	}

	void ActorEditor::displayBehaviours()
	{
		for (auto behaviour : _value["behaviours"])
		{
			auto editor = EditorRegister::createInstance(behaviour["typeID"], behaviour,
				[=](json pVal)
				{
					auto* b = dynamic_cast<Tristeon::Behaviour*>(Tristeon::InstanceCollector::find(pVal["instanceID"]));
					if (b)
					{
						b->deserialize(pVal);
						_value = b->actor()->serialize();
					}
				}
			);

			if (!editor)
			{
				editor = std::make_unique<BehaviourEditor>(behaviour, 
					[=](json pVal)
					{
						auto* b = dynamic_cast<Tristeon::Behaviour*>(Tristeon::InstanceCollector::find(pVal["instanceID"]));
						if (b)
						{
							b->deserialize(pVal);
							_value = b->actor()->serialize();
						}
					});
			}

			scrollArea->layout()->addWidget(editor->widget());
		}
		//for (auto* behaviour : actor->findBehaviours())
		//	addBehaviour(behaviour);

		//QPushButton* addBehaviour = new QPushButton(this);
		//addBehaviour->setStyleSheet("background-color: rgb(46, 204, 113);");
		//addBehaviour->setText("Add Behaviour");
		//layout->addWidget(addBehaviour);
		//connect(addBehaviour, &QPushButton::clicked, this, &ActorEditor::addButtonPressed);
	}

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

	//}
}
