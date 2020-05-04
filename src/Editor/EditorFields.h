#pragma once
#ifdef TRISTEON_EDITOR
#include <QtWidgets>

namespace TristeonEditor
{
	class EditorFields
	{
	public:
		static QDoubleSpinBox* floatField(QWidget* parent, float value, std::function<void(float)> changeCallback);
		static QDoubleSpinBox* floatField(QWidget* parent, float value, float minValue, float maxValue, std::function<void(float)> changeCallback);
		static QSpinBox* intField(QWidget* parent, int value, std::function<void(int)> changeCallback);
		static QSpinBox* intField(QWidget* parent, int value, int minValue, int maxValue, std::function<void(int)> changeCallback);
	};
}
#endif