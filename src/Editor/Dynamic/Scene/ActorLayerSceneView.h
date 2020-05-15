#pragma once
#ifdef TRISTEON_EDITOR
#include <QtWidgets>
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

	private:
		Tristeon::ActorLayer* actorLayer = nullptr;
		QLabel* highlight = nullptr;
	};
}
#endif