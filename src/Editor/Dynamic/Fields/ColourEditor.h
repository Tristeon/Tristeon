#pragma once
#include <qpushbutton.h>

#include "Editor/Dynamic/AbstractJsonEditor.h"
#include <Editor/Dynamic/EditorRegister.h>

#include <Utils/Colour.h>

namespace TristeonEditor
{
	class ColourEditor : public AbstractJsonEditor
	{
	public:
		ColourEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback);
		virtual void setValue(const nlohmann::json& pValue) override;

	private:
		QPushButton* _button = nullptr;
	};

	EDITOR(Tristeon::Colour, ColourEditor);
}
