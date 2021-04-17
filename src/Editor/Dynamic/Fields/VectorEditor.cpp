#include "VectorEditor.h"

namespace TristeonEditor
{
	VectorEditor::VectorEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback) : AbstractJsonEditor(pValue, pCallback)
	{
		_widget = new QWidget();
		auto* layout = new QHBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		_widget->setLayout(layout);

		_x = std::make_unique<FloatEditor>(pValue["x"],
			[=](nlohmann::json val)
			{
				_value["x"] = val;
				_callback(_value);
			}
		);
		layout->addWidget(_x->widget());

		_y = std::make_unique<FloatEditor>(pValue["y"],
			[=](nlohmann::json val)
			{
				_value["y"] = val;
				_callback(_value);
			}
		);
		layout->addWidget(_y->widget());
	}

	void VectorEditor::setValue(const nlohmann::json& pValue)
	{
		_x->setValue(pValue["x"]);
		_y->setValue(pValue["y"]);
	}
}