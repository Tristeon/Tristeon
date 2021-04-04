#pragma once
#include <Editor/Dynamic/AbstractJsonEditor.h>
#include <Editor/Dynamic/EditorRegister.h>
#include <Scenes/Actors/Actor.h>

#include <qwidget.h>
#include <qformlayout.h>

namespace TristeonEditor
{
	class ActorEditor : public AbstractJsonEditor
	{
	public:
		ActorEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback);
		~ActorEditor() override;
		virtual void setValue(const nlohmann::json& pValue) override;

	protected:
		QLayout* _layout;

		QWidget* _formWidget;
		QFormLayout* _formLayout;

		QLayout* scrollLayout = nullptr;
		QWidget* scrollArea = nullptr;

		std::map<std::string, Tristeon::Unique<AbstractJsonEditor>> _editors;

		void displayDefaultProperties();
		void displayAutoProperties();
		void displayBehaviours();
	};

	EDITOR(Tristeon::Actor, ActorEditor);
}