#pragma once
#ifdef TRISTEON_EDITOR
#include <Editor/Dynamic/Objects/ObjectEditor.h>
#include <Editor/Dynamic/Objects/ObjectEditorRegister.h>
#include <Scenes/Actors/Behaviour.h>

namespace TristeonEditor
{
	class BehaviourEditor : public ObjectEditor
	{
	public:
		void initialize() override;
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;

		virtual void displayContents();
	private:
		void removeButtonPressed();

		Tristeon::Behaviour* behaviour = nullptr;
		json data;
	};
	
	OBJECT_EDITOR(Tristeon::Behaviour, BehaviourEditor);
}
#endif