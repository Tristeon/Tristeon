#pragma once
#include "Editor/Dynamic/AbstractJsonEditor.h"
#include <Editor/Dynamic/EditorRegister.h>

#include <qspinbox.h>

namespace TristeonEditor
{
	class IntEditor : public AbstractJsonEditor
	{
	public:
		IntEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback);
		virtual void setValue(const nlohmann::json& pValue) override;
	private:
		QSpinBox* _spinbox = nullptr;
	};

	EDITOR(int, IntEditor);
}