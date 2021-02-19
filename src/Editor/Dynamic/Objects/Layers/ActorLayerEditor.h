#pragma once
#ifdef TRISTEON_EDITOR
#include <Editor/Dynamic/Objects/ObjectEditor.h>
#include <Editor/Dynamic/Objects/ObjectEditorRegister.h>
#include <Scenes/Layers/ActorLayer.h>
#include <QtWidgets>

namespace TristeonEditor
{
	class ActorLayerEditor : public ObjectEditor
	{
	public:
		virtual ~ActorLayerEditor();
		void initialize() override;
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

	OBJECT_EDITOR(Tristeon::ActorLayer, ActorLayerEditor);
}
#endif