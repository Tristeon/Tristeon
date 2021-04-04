#pragma once
#include <Math/Vector.h>

#include "Editor/Dynamic/AbstractJsonEditor.h"
#include <Editor/Dynamic/EditorRegister.h>

#include "UIntEditor.h"

namespace TristeonEditor
{
	class VectorUEditor : public AbstractJsonEditor
	{
	public:
		VectorUEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback);
		virtual void setValue(const nlohmann::json& pValue) override;

	private:
		Tristeon::Unique<UIntEditor> _x;
		Tristeon::Unique<UIntEditor> _y;
	};

	EDITOR(Tristeon::VectorU, VectorUEditor);
}