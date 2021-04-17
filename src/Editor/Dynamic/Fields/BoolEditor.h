#pragma once
#include <qcheckbox.h>

#include "Editor/Dynamic/AbstractJsonEditor.h"
#include <Editor/Dynamic/EditorRegister.h>

namespace TristeonEditor
{
	class BoolEditor : public AbstractJsonEditor
	{
	public:
		BoolEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback);
		virtual void setValue(const nlohmann::json& pValue) override;
	private:
		QCheckBox* _checkBox = nullptr;
	};

	EDITOR(bool, BoolEditor);
}