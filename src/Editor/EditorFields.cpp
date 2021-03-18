#ifdef TRISTEON_EDITOR
#include "EditorFields.h"

namespace TristeonEditor
{
	void EditorFields::vectorField(QFormLayout* layout, std::string label, Tristeon::Vector value, std::function<void(float)> changeCallbackX, std::function<void(float)> changeCallbackY)
	{
		auto* field = new QWidget();
		auto* l = new QHBoxLayout(field);
		l->setContentsMargins(0, 0, 0, 0);
		field->setLayout(layout);

		auto* x = floatField(field, value.x, changeCallbackX);
		auto* y = floatField(field, value.y, changeCallbackY);
		l->addWidget(x);
		l->addWidget(y);

		layout->addRow(new QLabel(QString::fromStdString(label)), field);
	}

	void EditorFields::vectorIField(QFormLayout* layout, std::string label, Tristeon::VectorI value,
		std::function<void(int)> changeCallbackX, std::function<void(int)> changeCallbackY)
	{
		auto* field = new QWidget();
		auto* l = new QHBoxLayout(field);
		l->setContentsMargins(0, 0, 0, 0);
		field->setLayout(layout);

		auto* x = intField(field, value.x, changeCallbackX);
		auto* y = intField(field, value.y, changeCallbackY);
		l->addWidget(x);
		l->addWidget(y);

		layout->addRow(new QLabel(QString::fromStdString(label)), field);
	}

	void EditorFields::vectorUField(QFormLayout* layout, std::string label, Tristeon::VectorU value,
		std::function<void(unsigned int)> changeCallbackX, std::function<void(unsigned int)> changeCallbackY)
	{
		auto* field = new QWidget();
		auto* l = new QHBoxLayout(field);
		l->setContentsMargins(0, 0, 0, 0);
		field->setLayout(layout);

		auto* x = uintField(field, value.x, changeCallbackX);
		auto* y = uintField(field, value.y, changeCallbackY);
		l->addWidget(x);
		l->addWidget(y);

		layout->addRow(new QLabel(QString::fromStdString(label)), field);
	}

	void EditorFields::vectorBField(QFormLayout* layout, std::string label, Tristeon::VectorB value,
		std::function<void(bool)> changeCallbackX, std::function<void(bool)> changeCallbackY)
	{
		auto* field = new QWidget();
		auto* l = new QHBoxLayout(field);
		l->setContentsMargins(0, 0, 0, 0);
		field->setLayout(layout);

		auto* x = boolField(field, value.x, changeCallbackX);
		auto* y = boolField(field, value.y, changeCallbackY);
		l->addWidget(x);
		l->addWidget(y);

		layout->addRow(new QLabel(QString::fromStdString(label)), field);
	}

	QDoubleSpinBox* EditorFields::floatField(QFormLayout* layout, std::string label, float value, std::function<void(float)> changeCallback)
	{
		return floatField(layout, label, value, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), changeCallback);
	}

	QDoubleSpinBox* EditorFields::floatField(QFormLayout* layout, std::string label, float value, float minValue, float maxValue, std::function<void(float)> changeCallback)
	{
		auto* field = floatField(nullptr, value, minValue, maxValue, changeCallback);
		layout->addRow(new QLabel(QString::fromStdString(label)), field);
		return field;
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

	QSpinBox* EditorFields::intField(QFormLayout* layout, std::string label, int value, std::function<void(int)> changeCallback)
	{
		return intField(layout, label, value, std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max(), changeCallback);
	}

	QSpinBox* EditorFields::intField(QFormLayout* layout, std::string label, int value, int minValue, int maxValue, std::function<void(int)> changeCallback)
	{
		auto* field = intField(nullptr, value, minValue, maxValue, changeCallback);
		layout->addRow(new QLabel(QString::fromStdString(label)), field);
		return field;
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

	QSpinBox* EditorFields::uintField(QFormLayout* layout, std::string label, uint value, std::function<void(uint)> changeCallback)
	{
		return intField(layout, label, value, 0, std::numeric_limits<int>::max(), changeCallback);
	}

	QSpinBox* EditorFields::uintField(QFormLayout* layout, std::string label, uint value, uint minValue, uint maxValue,
		std::function<void(uint)> changeCallback)
	{
		return intField(layout, label, value, minValue, maxValue, changeCallback);
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

	QCheckBox* EditorFields::boolField(QFormLayout* layout, std::string label, bool value, std::function<void(bool)> changeCallback)
	{
		auto* field = boolField(nullptr, value, std::move(changeCallback));
		layout->addRow(new QLabel(QString::fromStdString(label)), field);
		return field;
	}

	QCheckBox* EditorFields::boolField(QWidget* parent, bool value, std::function<void(bool)> changeCallback)
	{
		auto* field = new QCheckBox();
		field->setCheckState(value ? Qt::Checked : Qt::Unchecked);
		QWidget::connect(field, &QCheckBox::stateChanged, std::move(changeCallback));
		return field;
	}

	QLineEdit* EditorFields::stringField(QFormLayout* layout, std::string label, std::string value, std::function<void(const std::string&)> changeCallback)
	{
		auto* field = stringField(nullptr, value, changeCallback);
		layout->addRow(new QLabel(QString::fromStdString(label)), field);
		return field;
	}

	QLineEdit* EditorFields::stringField(QWidget* parent, std::string value,
		std::function<void(const std::string&)> changeCallback)
	{
		auto* field = new QLineEdit();
		field->setText(QString::fromStdString(value));
		QWidget::connect(field, &QLineEdit::textChanged, [=](const QString & val) { changeCallback(val.toStdString()); });
		return field;
	}

	QLabel* EditorFields::labelField(QFormLayout* layout, std::string label, std::string value)
	{
		auto* field = new QLabel(QString::fromStdString(value));
		layout->addRow(new QLabel(QString::fromStdString(label)), field);
		return field;
	}

	QLabel* EditorFields::header(QFormLayout* layout, std::string label)
	{
		auto widget = new QLabel(label.data());
		widget->setStyleSheet("font-weight: bold;");
		layout->addRow(widget, new QWidget());
		return widget;
	}
	
	QComboBox* EditorFields::dropdown(QFormLayout* layout, std::string label, int currentIndex, Tristeon::List<std::string> options, std::function<void(int)> changeCallback)
	{
		auto field = new QComboBox();
		for (auto option : options)
			field->addItem(option.c_str());
		field->setCurrentIndex(currentIndex);

		QWidget::connect(field, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), changeCallback);
		layout->addRow(new QLabel(label.c_str()), field);
		return field;
	}
}
#endif