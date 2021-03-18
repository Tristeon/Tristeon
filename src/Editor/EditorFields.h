#pragma once
#ifdef TRISTEON_EDITOR
#include <QtWidgets>
#include <Standard/List.h>
#include <Math/Vector.h>

namespace TristeonEditor
{
	class EditorFields
	{
	public:
		static void vectorField(QFormLayout* layout, std::string label, Tristeon::Vector value, std::function<void(float)> changeCallbackX, std::function<void(float)> changeCallbackY);

		static void vectorIField(QFormLayout* layout, std::string label, Tristeon::VectorI value, std::function<void(int)> changeCallbackX, std::function<void(int)> changeCallbackY);

		static void vectorUField(QFormLayout* layout, std::string label, Tristeon::VectorU value, std::function<void(unsigned int)> changeCallbackX, std::function<void(unsigned int)> changeCallbackY);

		static void vectorBField(QFormLayout* layout, std::string label, Tristeon::VectorB value, std::function<void(bool)> changeCallbackX, std::function<void(bool)> changeCallbackY);
		
		static QDoubleSpinBox* floatField(QFormLayout* layout, std::string label, float value, std::function<void(float)> changeCallback);
		static QDoubleSpinBox* floatField(QFormLayout* layout, std::string label, float value, float minValue, float maxValue, std::function<void(float)> changeCallback);
		static QDoubleSpinBox* floatField(QWidget* parent, float value, std::function<void(float)> changeCallback);
		static QDoubleSpinBox* floatField(QWidget* parent, float value, float minValue, float maxValue, std::function<void(float)> changeCallback);
		
		static QSpinBox* intField(QFormLayout* layout, std::string label, int value, std::function<void(int)> changeCallback);
		static QSpinBox* intField(QFormLayout* layout, std::string label, int value, int minValue, int maxValue, std::function<void(int)> changeCallback);
		static QSpinBox* intField(QWidget* parent, int value, std::function<void(int)> changeCallback);
		static QSpinBox* intField(QWidget* parent, int value, int minValue, int maxValue, std::function<void(int)> changeCallback);
		
		static QSpinBox* uintField(QFormLayout* layout, std::string label, uint value, std::function<void(uint)> changeCallback);
		static QSpinBox* uintField(QFormLayout* layout, std::string label, uint value, uint minValue, uint maxValue, std::function<void(uint)> changeCallback);
		static QSpinBox* uintField(QWidget* parent, uint value, std::function<void(uint)> changeCallback);
		static QSpinBox* uintField(QWidget* parent, uint value, uint minValue, uint maxValue, std::function<void(uint)> changeCallback);
		
		static QCheckBox* boolField(QFormLayout* layout, std::string label, bool value, std::function<void(bool)> changeCallback);
		static QCheckBox* boolField(QWidget* parent, bool value, std::function<void(bool)> changeCallback);

		static QLineEdit* stringField(QFormLayout* layout, std::string label, std::string value, std::function<void(const std::string&)> changeCallback);
		static QLineEdit* stringField(QWidget* parent, std::string value, std::function<void(const std::string&)> changeCallback);

		static QLabel* labelField(QFormLayout* layout, std::string label, std::string value);
		static QLabel* header(QFormLayout* layout, std::string label);

		static QComboBox* dropdown(QFormLayout* layout, std::string label, int currentIndex, Tristeon::List<std::string> options, std::function<void(int)> changeCallback);
	};
}
#endif