#pragma once
#ifdef TRISTEON_EDITOR
#include <QtWidgets>
#include "SceneEditor.h"
#include "SceneEditorRegister.h"
#include "Rendering/Graphic.h"
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

		Tristeon::Graphic::Bounds scalar{};
		Tristeon::Graphic::Bounds rotator{};
		
		bool dragging = false;
		bool draggingScalar = false;
		bool draggingRotate = false;

		Tristeon::Vector2 draggingOffset{};
	};
}
#endif