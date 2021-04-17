#include "BoolEditor.h"

#include <qboxlayout.h>

namespace TristeonEditor
{
	BoolEditor::BoolEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback): AbstractJsonEditor(pValue, pCallback)
	{
		_checkBox = new QCheckBox();
		_widget = _checkBox;
		_checkBox->setCheckState(pValue.get<bool>() ? Qt::Checked : Qt::Unchecked);
		QWidget::connect(_checkBox, &QCheckBox::stateChanged, [=](bool val) { _callback(val); });
	}

	void BoolEditor::setValue(const nlohmann::json& pValue)
	{
		_checkBox->setCheckState(pValue.get<bool>() ? Qt::Checked : Qt::Unchecked);
	}
}