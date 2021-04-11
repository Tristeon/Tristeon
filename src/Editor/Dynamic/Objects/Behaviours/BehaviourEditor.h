#pragma once
#include <Editor/Dynamic/AbstractJsonEditor.h>
#include <Standard/Unique.h>

namespace TristeonEditor
{
	class BehaviourEditor : public AbstractJsonEditor
	{
	public:
		BehaviourEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback);
		virtual void setValue(const nlohmann::json& pValue) override;
	private:
		std::map<std::string, Tristeon::Unique<AbstractJsonEditor>> _editors;
	};
}