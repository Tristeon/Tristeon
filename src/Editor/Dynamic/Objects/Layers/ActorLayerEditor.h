#pragma once
#include <Editor/Dynamic/AbstractJsonEditor.h>
#include <Editor/Dynamic/EditorRegister.h>

#include <Scenes/Layers/ActorLayer.h>
#include <QtWidgets>

namespace TristeonEditor
{
	class ActorLayerEditor : public AbstractJsonEditor
	{
	public:
		ActorLayerEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback);
		virtual ~ActorLayerEditor() override;
		virtual void setValue(const nlohmann::json& pValue) override;
	private:
		void selectedActorChanged(int index);
		void actorRenamed(QListWidgetItem* item);

		void addActor();
		void removeActor();

		int nameChangedCallback = -1;
		QListWidget* list = nullptr;
		std::map<QListWidgetItem*, Tristeon::Actor*> actors; //TODO: Storing direct pointers to actors here can cause instability
	};

	EDITOR(Tristeon::ActorLayer, ActorLayerEditor);
}