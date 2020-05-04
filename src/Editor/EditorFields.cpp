#ifdef TRISTEON_EDITOR
#include "EditorFields.h"

namespace TristeonEditor
{
	QDoubleSpinBox* EditorFields::floatField(QWidget* parent, float value, std::function<void(float)> changeCallback)
	{
		return floatField(parent, value, std::numeric_limits<float>::min(), std::numeric_limits<float>::max(), changeCallback);
	}

	QDoubleSpinBox* EditorFields::floatField(QWidget* parent, float value, float minValue, float maxValue, std::function<void(float)> changeCallback)
	{
		auto* field = new QDoubleSpinBox(parent);
		field->setFocusPolicy(Qt::StrongFocus);
		field->setSingleStep(0);
		field->setButtonSymbols(QDoubleSpinBox::NoButtons);
		field->setMinimum(minValue);
		field->setMaximum(maxValue);
		field->setValue(value);
		QWidget::connect(field, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), changeCallback);
		return field;
	}

	QSpinBox* EditorFields::intField(QWidget* parent, int value, std::function<void(int)> changeCallback)
	{
		return intField(parent, value, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), changeCallback);
	}

	QSpinBox* EditorFields::intField(QWidget* parent, int value, int minValue, int maxValue, std::function<void(int)> changeCallback)
	{
		auto* field = new QSpinBox(parent);
		field->setFocusPolicy(Qt::StrongFocus);
		field->setMinimum(minValue);
		field->setMaximum(maxValue);
		field->setSingleStep(0);
		field->setButtonSymbols(QDoubleSpinBox::NoButtons);
		field->setValue(value);
		QWidget::connect(field, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), changeCallback);
		return field;
	}
}
#endif