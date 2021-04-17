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

		_scroll = new QScrollArea();
		_scroll->setFrameStyle(QFrame::NoFrame);
		_scroll->setAlignment(Qt::AlignTop);
		_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		_scroll->setWidgetResizable(true);

		_scrollWidget = new QWidget();
		_scrollLayout = new QVBoxLayout();
		_scrollLayout->setAlignment(Qt::AlignTop);
		_scrollLayout->setContentsMargins(0, 0, 0, 0);
		_scrollWidget->setLayout(_scrollLayout);

		//Display manual properties first, then automatic properties, then behaviours.
		//Each of these should be using the scroll area
		displayAutoProperties();
		displayBehaviours();

		_scroll->setWidget(_scrollWidget);
		_layout->addWidget(_scroll);

		QPushButton* addBehaviour = new QPushButton();
		addBehaviour->setStyleSheet("background-color: rgb(46, 204, 113);");
		addBehaviour->setText("Add Behaviour");
		_layout->addWidget(addBehaviour);
		QWidget::connect(addBehaviour, &QPushButton::clicked, [=]() { addButtonPressed(); });
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

	void ActorEditor::removeBehaviourEditor(BehaviourEditor* editor)
	{
		for (auto i = 0; i < _behaviourEditors.size(); i++)
		{
			if (_behaviourEditors[i].get() == editor)
			{
				_behaviourEditors[i].reset();
				_behaviourEditors.removeAt(i);
			}
		}
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
		auto* formWidget = new QWidget();
		_scrollLayout->addWidget(formWidget);
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
			addBehaviourEditor(behaviour);
		}
	}

	void ActorEditor::addButtonPressed()
	{
		QMenu contextMenu(QWidget::tr("Context menu"));

		auto* map = Tristeon::Register<Tristeon::Behaviour>::getMap();
		for (auto const& pair : *map)
		{
			QAction* action = new QAction(pair.first.c_str());
			QWidget::connect(action, &QAction::triggered, [=](bool checked)
				{
					Tristeon::Actor* actor = dynamic_cast<Tristeon::Actor*>(Tristeon::InstanceCollector::find(_value["instanceID"]));
					if (actor == nullptr)
						return;
				
					auto* behaviour = actor->createBehaviour(pair.first);
					addBehaviourEditor(behaviour->serialize());

					_value = actor->serialize();
					_callback(_value);
				});
			contextMenu.addAction(action);
		}

		contextMenu.exec(QCursor::pos());
	}

	void ActorEditor::addBehaviourEditor(json behaviour)
	{
		auto editor = EditorRegister::createInstance(behaviour["typeID"], behaviour,
			[=](json pVal)
			{
				auto* b = dynamic_cast<Tristeon::Behaviour*>(Tristeon::InstanceCollector::find(pVal["instanceID"]));
				if (b)
				{
					b->deserialize(pVal);
					_value = b->actor()->serialize();
					_callback(_value);
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
						_callback(_value);
					}
				});
		}

		BehaviourEditor* be = dynamic_cast<BehaviourEditor*>(editor.get());
		if (be)
			be->actorEditor = this;
		
		_scrollLayout->addWidget(editor->widget());
		_behaviourEditors.add(std::move(editor));
	}
}
