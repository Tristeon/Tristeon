#ifdef TRISTEON_EDITOR
#include "EditorFields.h"

namespace TristeonEditor
{
	void EditorFields::floatField(QFormLayout* layout, std::string label, float value, std::function<void(float)> changeCallback)
	{
		floatField(layout, label, value, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), changeCallback);
	}

	void EditorFields::floatField(QFormLayout* layout, std::string label, float value, float minValue, float maxValue, std::function<void(float)> changeCallback)
	{
		auto* field = floatField(nullptr, value, minValue, maxValue, changeCallback);
		layout->addRow(new QLabel(QString::fromStdString(label)), field);
	}

	QDoubleSpinBox* EditorFields::floatField(QWidget* parent, float value, std::function<void(float)> changeCallback)
	{
		return floatField(parent, value, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), changeCallback);
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

	void EditorFields::intField(QFormLayout* layout, std::string label, int value, std::function<void(int)> changeCallback)
	{
		intField(layout, label, value, std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max(), changeCallback);
	}

	void EditorFields::intField(QFormLayout* layout, std::string label, int value, int minValue, int maxValue, std::function<void(int)> changeCallback)
	{
		auto* field = intField(nullptr, value, changeCallback);
		layout->addRow(new QLabel(QString::fromStdString(label)), field);
	}

	QSpinBox* EditorFields::intField(QWidget* parent, int value, std::function<void(int)> changeCallback)
	{
		return intField(parent, value, std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max(), changeCallback);
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

	void EditorFields::uintField(QFormLayout* layout, std::string label, uint value, std::function<void(uint)> changeCallback)
	{
		intField(layout, label, value, 0, std::numeric_limits<int>::max(), changeCallback);
	}

	void EditorFields::uintField(QFormLayout* layout, std::string label, uint value, uint minValue, uint maxValue,
		std::function<void(uint)> changeCallback)
	{
		intField(layout, label, value, minValue, maxValue, changeCallback);
	}

	QSpinBox* EditorFields::uintField(QWidget* parent, uint value, std::function<void(uint)> changeCallback)
	{
		return intField(parent, value, changeCallback);
	}

	QSpinBox* EditorFields::uintField(QWidget* parent, uint value, uint minValue, uint maxValue,
		std::function<void(uint)> changeCallback)
	{
		return intField(parent, value, minValue, maxValue, changeCallback);
	}

	void EditorFields::boolField(QFormLayout* layout, std::string label, bool value, std::function<void(bool)> changeCallback)
	{
		auto* field = boolField(nullptr, value, std::move(changeCallback));
		layout->addRow(new QLabel(QString::fromStdString(label)), field);
	}

	QCheckBox* EditorFields::boolField(QWidget* parent, bool value, std::function<void(bool)> changeCallback)
	{
		auto* field = new QCheckBox();
		field->setCheckState(value ? Qt::Checked : Qt::Unchecked);
		QWidget::connect(field, &QCheckBox::stateChanged, std::move(changeCallback));
		return field;
	}

	void EditorFields::stringField(QFormLayout* layout, std::string label, std::string value, std::function<void(const std::string&)> changeCallback)
	{
		auto* field = stringField(nullptr, value, changeCallback);
		layout->addRow(new QLabel(QString::fromStdString(label)), field);
	}

	QLineEdit* EditorFields::stringField(QWidget* parent, std::string value,
		std::function<void(const std::string&)> changeCallback)
	{
		auto* field = new QLineEdit();
		field->setText(QString::fromStdString(value));
		QWidget::connect(field, &QLineEdit::textChanged, [=](const QString & val) { changeCallback(val.toStdString()); });
		return field;
	}

	void EditorFields::labelField(QFormLayout* layout, std::string label, std::string value)
	{
		layout->addRow(new QLabel(QString::fromStdString(label)), new QLabel(QString::fromStdString(value)));
	}
}
#endif