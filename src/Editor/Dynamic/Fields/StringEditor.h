#pragma once
#include "Editor/Dynamic/AbstractJsonEditor.h"
#include <Editor/Dynamic/EditorRegister.h>

#include <qlineedit.h>

namespace TristeonEditor
{
	class StringEditor : public AbstractJsonEditor
	{
	public:
		StringEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback);
		virtual void setValue(const nlohmann::json& pValue) override;

	private:
		QLineEdit* _lineEdit = nullptr;
	};

	EDITOR(std::string, StringEditor);
}