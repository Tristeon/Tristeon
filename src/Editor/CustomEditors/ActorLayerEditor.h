#pragma once
#ifdef TRISTEON_EDITOR
#include "CustomEditor.h"
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
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
	private:
		void selectedActorChanged(int index);
		void actorRenamed(QListWidgetItem* item);
		
		Tristeon::ActorLayer* targetLayer = nullptr;
		QListWidget* list = nullptr;
		std::map<QListWidgetItem*, Tristeon::Actor*> actors;
	};
}
#endif