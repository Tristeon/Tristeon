#pragma once
#include <Math/Vector.h>

#include "Editor/Dynamic/AbstractJsonEditor.h"
#include <Editor/Dynamic/EditorRegister.h>

#include "IntEditor.h"

namespace TristeonEditor
{
	class VectorIEditor : public AbstractJsonEditor
	{
	public:
		VectorIEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback);
		virtual void setValue(const nlohmann::json& pValue) override;

	private:
		Tristeon::Unique<IntEditor> _x;
		Tristeon::Unique<IntEditor> _y;
	};

	EDITOR(Tristeon::VectorI, VectorIEditor);
}