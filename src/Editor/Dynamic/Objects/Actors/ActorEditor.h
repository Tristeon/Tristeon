#pragma once
#include "Editor/Dynamic/Fields/FieldEditor.h"
#ifdef TRISTEON_EDITOR
#include <Editor/Dynamic/Objects/ObjectEditor.h>
#include <Editor/Dynamic/Objects/ObjectEditorRegister.h>
#include <Scenes/Actors/Actor.h>

namespace TristeonEditor
{
	class ActorEditor : public ObjectEditor
	{
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

		Tristeon::List<Tristeon::Unique<FieldEditor>> fieldEditors;
		Tristeon::Actor* actor = nullptr;

		json data;
	};

	OBJECT_EDITOR(Tristeon::Actor, ActorEditor);
}
#endif