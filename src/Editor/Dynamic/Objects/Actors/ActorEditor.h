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

		void displayActorProperties();
		virtual void displayProperties();
		void displayAutoProperties();
		void displayBehaviours();

		/**
		 * If true, the actor editor automatically displays the property after all other properties have been displayed
		 */
		virtual bool shouldDisplay(Tristeon::String const& propertyName);

	protected:
		QLayout* scrollLayout = nullptr;
		QWidget* scrollArea = nullptr;
		
	private:
		void addBehaviour(Tristeon::Behaviour* behaviour);
		void actorNameChanged(const QString& name);
		void addButtonPressed();

		Tristeon::Actor* actor = nullptr;

		json data;
	};
}
#endif