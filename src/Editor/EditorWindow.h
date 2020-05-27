#pragma once
#ifdef TRISTEON_EDITOR
#include "Dynamic/Files/FileItem.h"
#include <Scenes/Scene.h>
#include <Actors/Actor.h>

namespace TristeonEditor
{
	class EditorWindow
	{
		friend class Editor;
	public:
		EditorWindow() = default;
		virtual ~EditorWindow() = default;

	protected:
		virtual void initialize() = 0;
		virtual void sceneLoaded(Tristeon::Scene* scene) = 0;
		virtual void selectedLayerChanged(Tristeon::Layer* layer) { /* Empty */ }
		virtual void selectedActorChanged(Tristeon::Actor* actor) { /* Empty */ }
		virtual void selectedFilePathChanged(FileItem* item) { /* Empty */ }
	};
}
#endif