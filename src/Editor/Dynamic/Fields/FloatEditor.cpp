#include "FloatEditor.h"

namespace TristeonEditor
{
	FloatEditor::FloatEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback): AbstractJsonEditor(pValue, pCallback)
	{
		_spinbox = new QDoubleSpinBox();
		_widget = _spinbox;
		_spinbox->setFocusPolicy(Qt::StrongFocus);
		_spinbox->setSingleStep(0);
		_spinbox->setButtonSymbols(QDoubleSpinBox::NoButtons);
		_spinbox->setMinimum(std::numeric_limits<float>::lowest());
		_spinbox->setMaximum(std::numeric_limits<float>::max());
		_spinbox->setValue(pValue.get<float>());
		QWidget::connect(_spinbox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
		                 [=](double val) { _callback((float)val); });
	}

	void FloatEditor::setValue(const nlohmann::json& pValue)
	{
		_spinbox->setValue(pValue.get<float>());
	}
}