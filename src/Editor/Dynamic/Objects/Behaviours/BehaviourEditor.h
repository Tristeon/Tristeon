#pragma once
#include <Editor/Dynamic/AbstractJsonEditor.h>
#include <Standard/Unique.h>

namespace TristeonEditor
{
	class ActorEditor;

	class BehaviourEditor : public AbstractJsonEditor
	{
	public:
		BehaviourEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback);
		virtual void setValue(const nlohmann::json& pValue) override;

		ActorEditor* actorEditor = nullptr;
	private:
		std::map<std::string, Tristeon::Unique<AbstractJsonEditor>> _editors;
	};
}
