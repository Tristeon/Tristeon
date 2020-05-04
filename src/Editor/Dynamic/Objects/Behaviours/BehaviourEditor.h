#pragma once
#ifdef TRISTEON_EDITOR
#include <Editor/Dynamic/Objects/ObjectEditor.h>
#include <Editor/Dynamic/Objects/ObjectEditorRegister.h>
#include <Actors/Behaviours/Behaviour.h>
#include <QtWidgets>

namespace TristeonEditor
{
	class BehaviourEditor : public ObjectEditor
	{
		OBJECT_EDITOR_H(Tristeon::Behaviour, BehaviourEditor);
	public:
		void initialize() override;
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;

		virtual void displayContents();
	private:
		void removeButtonPressed();

		Tristeon::Behaviour* behaviour = nullptr;
		json data;
	};
}
#endif