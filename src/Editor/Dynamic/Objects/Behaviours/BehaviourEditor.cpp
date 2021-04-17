#include "BehaviourEditor.h"

#include <qformlayout.h>
#include <qlabel.h>
#include <qtoolbutton.h>
#include <Standard/String.h>
#include <Serialization/Type.h>

#include <Editor/Dynamic/EditorRegister.h>


#include "InstanceCollector.h"
#include "Editor/Dynamic/Objects/Actors/ActorEditor.h"
#include "Scenes/Actors/Behaviour.h"

namespace TristeonEditor
{
	BehaviourEditor::BehaviourEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback) : AbstractJsonEditor(pValue, pCallback)
	{
		_widget = new QWidget();
		_widget->setLayout(new QVBoxLayout());
		_widget->layout()->setContentsMargins(0, 0, 0, 0);
		_widget->setContentsMargins(0, 0, 0, 0);

		auto* titleBar = new QWidget();
		titleBar->setStyleSheet("background-color: rgb(22, 160, 133);");
		_widget->layout()->addWidget(titleBar);

		auto* titleLayout = new QHBoxLayout(titleBar);
		titleLayout->setContentsMargins(0, 0, 0, 0);
		titleBar->setLayout(titleLayout);

		auto* typeName = new QLabel(QString::fromStdString(_value["typeID"]), titleBar);
		typeName->setAlignment(Qt::AlignLeft);
		titleLayout->addWidget(typeName);

		auto* closeButton = new QToolButton(titleBar);
		closeButton->setIcon(QIcon(QPixmap(QString("Internal/Icons/cross.jpg"))));
		closeButton->setMaximumSize(15, 15);
		titleLayout->addWidget(closeButton);
		QWidget::connect(closeButton, &QToolButton::clicked, [=]()
			{
				Tristeon::Behaviour* behaviour = dynamic_cast<Tristeon::Behaviour*>(Tristeon::InstanceCollector::find(_value["instanceID"]));
				behaviour->destroy();

				actorEditor->removeBehaviourEditor(this);
			});

		QWidget* formWidget = new QWidget();
		_widget->layout()->addWidget(formWidget);
		auto* form = new QFormLayout();
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
			case nlohmann::detail::value_t::object:
				type = it.value().value("typeID", "");
				break;
			case nlohmann::detail::value_t::array:
				type = "";
				break;
			case nlohmann::detail::value_t::string:
				type = Tristeon::Type<Tristeon::String>::fullName();
				break;
			case nlohmann::detail::value_t::boolean:
				type = Tristeon::Type<bool>::fullName();
				break;
			case nlohmann::detail::value_t::number_integer:
				type = Tristeon::Type<int>::fullName();
				break;
			case nlohmann::detail::value_t::number_unsigned:
				type = Tristeon::Type<unsigned>::fullName();
				break;
			case nlohmann::detail::value_t::number_float:
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

	void BehaviourEditor::setValue(const nlohmann::json& pValue)
	{
		//TODO
	}
}
