#pragma once
#include <qpushbutton.h>

#include "Editor/Dynamic/AbstractJsonEditor.h"
#include <Editor/Dynamic/EditorRegister.h>

#include <Serialization/MetaWrappers/TexturePath.h>

namespace TristeonEditor
{
	class TexturePathEditor : public AbstractJsonEditor
	{
	public:
		TexturePathEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback);
		virtual void setValue(const nlohmann::json& pValue) override;
	private:
		QPushButton* _button = nullptr;
	};

	EDITOR(Tristeon::TexturePath, TexturePathEditor);
}