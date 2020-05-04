#pragma once
#ifdef TRISTEON_EDITOR
#include <Editor/Dynamic/Objects/ObjectEditor.h>
#include <Editor/Dynamic/Objects/ObjectEditorRegister.h>
#include <Actors/Actor.h>
namespace TristeonEditor
{
	class ActorEditor : public ObjectEditor
	{
		OBJECT_EDITOR_H(Tristeon::Actor, ActorEditor)
	public:
		void initialize() override;
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
		virtual void displayProperties();

	private:
		void displayBehaviours();
		void addBehaviour(Tristeon::Behaviour* behaviour);

		void actorNameChanged(const QString& name);
		void addButtonPressed();

		QWidget* behavioursArea = nullptr;
		Tristeon::Actor* actor = nullptr;
	};
}
#endif