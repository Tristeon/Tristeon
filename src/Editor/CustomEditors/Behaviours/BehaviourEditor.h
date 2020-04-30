#pragma once
#ifdef TRISTEON_EDITOR
#include <Editor/CustomEditors/CustomEditor.h>
#include <Editor/CustomEditors/EditorRegister.h>
#include <Actors/Behaviours/Behaviour.h>

namespace TristeonEditor
{
	class BehaviourEditor : public CustomEditor
	{
		CUSTOM_EDITOR_H(Tristeon::Behaviour, BehaviourEditor)
	public:
		void initialize() override;
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
	private:
		Tristeon::Behaviour* behaviour = nullptr;
		json data;
	};
}
#endif