#pragma once
#ifdef TRISTEON_EDITOR
#include <QtWidgets>

namespace TristeonEditor
{
	class EditorFields
	{
	public:
		static void floatField(QFormLayout* layout, std::string label, float value, std::function<void(float)> changeCallback);
		static void floatField(QFormLayout* layout, std::string label, float value, float minValue, float maxValue, std::function<void(float)> changeCallback);
		static QDoubleSpinBox* floatField(QWidget* parent, float value, std::function<void(float)> changeCallback);
		static QDoubleSpinBox* floatField(QWidget* parent, float value, float minValue, float maxValue, std::function<void(float)> changeCallback);
		
		static void intField(QFormLayout* layout, std::string label, int value, std::function<void(int)> changeCallback);
		static void intField(QFormLayout* layout, std::string label, int value, int minValue, int maxValue, std::function<void(int)> changeCallback);
		static QSpinBox* intField(QWidget* parent, int value, std::function<void(int)> changeCallback);
		static QSpinBox* intField(QWidget* parent, int value, int minValue, int maxValue, std::function<void(int)> changeCallback);
		
		static void uintField(QFormLayout* layout, std::string label, uint value, std::function<void(uint)> changeCallback);
		static void uintField(QFormLayout* layout, std::string label, uint value, uint minValue, uint maxValue, std::function<void(uint)> changeCallback);
		static QSpinBox* uintField(QWidget* parent, uint value, std::function<void(uint)> changeCallback);
		static QSpinBox* uintField(QWidget* parent, uint value, uint minValue, uint maxValue, std::function<void(uint)> changeCallback);
		
		static void boolField(QFormLayout* layout, std::string label, bool value, std::function<void(int)> changeCallback);
		static QCheckBox* boolField(QWidget* parent, bool value, std::function<void(int)> changeCallback);

		static void stringField(QFormLayout* layout, std::string label, std::string value, std::function<void(const std::string&)> changeCallback);
		static QLineEdit* stringField(QWidget* parent, std::string value, std::function<void(const std::string&)> changeCallback);

		static void labelField(QFormLayout* layout, std::string label, std::string value);
	};
}
#endif