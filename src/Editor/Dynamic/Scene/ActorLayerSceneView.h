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
		void clickActor();
		
		Tristeon::ActorLayer* actorLayer = nullptr;

		QLabel* outline = nullptr;
		QLabel* corner = nullptr;
		QLabel* rotate = nullptr;
		
		bool dragging = false;
		bool draggingCorner = false;
		bool draggingRotate = false;

		Tristeon::Vector2 draggingOffset;
	};
}
#endif