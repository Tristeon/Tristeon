#pragma once
#ifdef TRISTEON_EDITOR
#include <Editor/CustomEditors/CustomEditor.h>
#include <Editor/CustomEditors/EditorRegister.h>
#include <Actors/Actor.h>
namespace TristeonEditor
{
	class ActorEditor : public CustomEditor
	{
		CUSTOM_EDITOR_H(Tristeon::Actor, ActorEditor)
	public:
		void initialize() override;
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;

	protected:
		virtual void displayProperties();
		void displayBehaviours();

		void actorNameChanged(const QString& name);

		Tristeon::Vector<QWidget*> behaviourWidgets;
		Tristeon::Actor* actor = nullptr;
	};
}
#endif