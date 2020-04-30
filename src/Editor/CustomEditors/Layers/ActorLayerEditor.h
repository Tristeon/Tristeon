#pragma once
#ifdef TRISTEON_EDITOR
#include <Editor/CustomEditors/CustomEditor.h>
#include <Scenes/Layers/ActorLayer.h>
#include <Editor/CustomEditors/EditorRegister.h>
#include <QtWidgets>

namespace TristeonEditor
{
	class ActorLayerEditor : public CustomEditor
	{
		CUSTOM_EDITOR_H(Tristeon::ActorLayer, ActorLayerEditor)
	public:
		void initialize() override;
		virtual ~ActorLayerEditor();
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
	private:
		void selectedActorChanged(int index);
		void actorRenamed(QListWidgetItem* item);

		void addActor();
		void removeActor();

		int nameChangedCallback = -1;
		
		Tristeon::ActorLayer* targetLayer = nullptr;
		QListWidget* list = nullptr;
		std::map<QListWidgetItem*, Tristeon::Actor*> actors;
	};
}
#endif