#pragma once
#include <Math/Vector.h>

#include "Editor/Dynamic/AbstractJsonEditor.h"
#include <Editor/Dynamic/EditorRegister.h>

#include <qboxlayout.h>

#include "FloatEditor.h"

namespace TristeonEditor
{
	class VectorEditor : public AbstractJsonEditor
	{
	public:
		VectorEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback);
		virtual void setValue(const nlohmann::json& pValue) override;

	private:
		Tristeon::Unique<FloatEditor> _x;
		Tristeon::Unique<FloatEditor> _y;
	};

	EDITOR(Tristeon::Vector, VectorEditor);
}
