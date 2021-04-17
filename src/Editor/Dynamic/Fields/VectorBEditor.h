#pragma once
#include <Math/Vector.h>

#include "Editor/Dynamic/AbstractJsonEditor.h"
#include <Editor/Dynamic/EditorRegister.h>

#include "BoolEditor.h"

namespace TristeonEditor
{
	class VectorBEditor : public AbstractJsonEditor
	{
	public:
		VectorBEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback);
		virtual void setValue(const nlohmann::json& pValue) override;

	private:
		Tristeon::Unique<BoolEditor> _x;
		Tristeon::Unique<BoolEditor> _y;
	};

	EDITOR(Tristeon::VectorB, VectorBEditor);
}