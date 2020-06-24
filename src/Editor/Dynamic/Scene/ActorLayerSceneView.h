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

		Tristeon::Graphic::AABB scalar;
		Tristeon::Graphic::AABB rotator;
		
		bool dragging = false;
		bool dragginScale = false;
		bool draggingRotate = false;

		Tristeon::Vector2 draggingOffset;
	};
}
#endif