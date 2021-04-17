#pragma once
#include <Editor/Dynamic/AbstractJsonEditor.h>
#include <Editor/Dynamic/EditorRegister.h>
#include <Scenes/Actors/Actor.h>

#include <qwidget.h>
#include <qformlayout.h>
#include <qscrollarea.h>

namespace TristeonEditor
{
	class BehaviourEditor;

	class ActorEditor : public AbstractJsonEditor
	{
	public:
		ActorEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback);
		~ActorEditor() override;
		virtual void setValue(const nlohmann::json& pValue) override;

		void removeBehaviourEditor(BehaviourEditor* editor);
	protected:
		QLayout* _layout = nullptr;

		QWidget* _formWidget = nullptr;
		QFormLayout* _formLayout = nullptr;

		QScrollArea* _scroll = nullptr;
		QWidget* _scrollWidget = nullptr;
		QLayout* _scrollLayout = nullptr;
		
		std::map<std::string, Tristeon::Unique<AbstractJsonEditor>> _editors;

		Tristeon::List<Tristeon::Unique<AbstractJsonEditor>> _behaviourEditors;
		
		void displayDefaultProperties();
		void displayAutoProperties();
		void displayBehaviours();

		void addButtonPressed();
		void addBehaviourEditor(json behaviour);
	};

	EDITOR(Tristeon::Actor, ActorEditor);
}
