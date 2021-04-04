#pragma once
#include "Editor/Dynamic/AbstractJsonEditor.h"
#include <Editor/Dynamic/EditorRegister.h>

#include <qspinbox.h>

namespace TristeonEditor
{
	class FloatEditor : public AbstractJsonEditor
	{
	public:
		FloatEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback);
		virtual void setValue(const nlohmann::json& pValue) override;
	private:
		QDoubleSpinBox* _spinbox = nullptr;
	};

	EDITOR(float, FloatEditor);
}