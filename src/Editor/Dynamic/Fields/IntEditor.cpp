#include "IntEditor.h"

namespace TristeonEditor
{
	IntEditor::IntEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback): AbstractJsonEditor(pValue, pCallback)
	{
		_spinbox = new QSpinBox();
		_widget = _spinbox;
		_spinbox->setFocusPolicy(Qt::StrongFocus);
		_spinbox->setSingleStep(0);
		_spinbox->setButtonSymbols(QSpinBox::NoButtons);
		_spinbox->setMinimum(std::numeric_limits<int>::lowest());
		_spinbox->setMaximum(std::numeric_limits<int>::max());
		_spinbox->setValue(pValue.get<int>());
		QWidget::connect(_spinbox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		                 [=](int val) { _callback(val); });
	}

	void IntEditor::setValue(const nlohmann::json& pValue)
	{
		_spinbox->setValue(pValue.get<int>());
	}
}