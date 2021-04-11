#include "BehaviourEditor.h"

#include <qformlayout.h>
#include <qlabel.h>
#include <Standard/String.h>
#include <Serialization/Type.h>

#include "Editor/Dynamic/EditorRegister.h"

namespace TristeonEditor
{
	BehaviourEditor::BehaviourEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback) : AbstractJsonEditor(pValue, pCallback)
	{
		_widget = new QWidget();

		auto* form = new QFormLayout();
		form->setContentsMargins(0, 0, 0, 0);
		_widget->setLayout(form);

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