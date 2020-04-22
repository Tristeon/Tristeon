#pragma once
#ifdef TRISTEON_EDITOR
#include <Scenes/Scene.h>

namespace TristeonEditor
{
	class EditorWindow
	{
		friend class Editor;
		
	protected:
		virtual void initialize() = 0;
		virtual void loadScene(Tristeon::Scene* scene) = 0;
	};
}
#endif