#pragma once
#ifdef TRISTEON_EDITOR
#include "SceneEditor.h"
#include "SceneEditorRegister.h"
#include "Scenes/Layers/ActorLayer.h"

namespace TristeonEditor
{
	class ActorLayerSceneView : public SceneEditor
	{
		SCENE_EDITOR_H(Tristeon::ActorLayer, ActorLayerSceneView)
	public:
		void initialize() override;
		void updateView() override;
	};
}
#endif