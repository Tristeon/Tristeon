#pragma once
#ifdef TRISTEON_EDITOR
#include <QDockWidget>
#include <Scenes/Scene.h>

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
		virtual void loadScene(Tristeon::Scene* scene) = 0;
		virtual void selectedLayerChanged(Tristeon::Layer* layer) { }

		Editor* editor = nullptr;
	};
}
#endif