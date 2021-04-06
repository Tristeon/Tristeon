#pragma once
#include <Editor/Dynamic/AbstractJsonEditor.h>

#include <qcombobox.h>
#include <json.h>
#include <magic_enum.hpp>

namespace TristeonEditor
{
	template<typename T>
	class EnumEditor : public AbstractJsonEditor
	{
	public:
		EnumEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback);
		void setValue(const nlohmann::json& pValue) override;

	private:
		QComboBox* _comboBox;
	};

	template<typename T>
	EnumEditor<T>::EnumEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback) : AbstractJsonEditor(pValue, pCallback)
	{
		T current = pValue["value"];

		_comboBox = new QComboBox();
		_widget = _comboBox;

		constexpr const auto& names = magic_enum::enum_names<T>();
		for (auto name : names)
			_comboBox->addItem(name.data());

		auto currentIndex = magic_enum::enum_index<T>(current);
		_comboBox->setCurrentIndex(currentIndex.has_value() ? currentIndex.value() : 0);

		QWidget::connect(_comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), 
			[=](int idx) 
			{
				_value["value"] = magic_enum::enum_value<T>(idx);
				_callback(_value);
			}
		);
	}

	template <typename T>
	void EnumEditor<T>::setValue(const nlohmann::json& pValue)
	{
		_value = pValue;

		auto val = magic_enum::enum_index(_value["value"].get<T>());
		_comboBox->setCurrentIndex(val.has_value() ? val.value() : 0);
	}
}