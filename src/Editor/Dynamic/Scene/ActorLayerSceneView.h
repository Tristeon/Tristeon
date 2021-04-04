#pragma once
#include "SceneEditor.h"
#include "Rendering/Graphic.h"

#include <Scenes/Layers/ActorLayer.h>
#include <Editor/Dynamic/Scene/SceneEditorRegister.h>

namespace TristeonEditor
{
	class ActorLayerSceneView : public SceneEditor
	{
	public:
		explicit ActorLayerSceneView(Tristeon::Layer* pLayer) : SceneEditor(pLayer), _actorLayer(dynamic_cast<Tristeon::ActorLayer*>(pLayer)) { }
		void updateView() override;

	private:
		void clickActor();
		
		Tristeon::ActorLayer* _actorLayer = nullptr;

		Tristeon::Graphic::Bounds _scalar{};
		Tristeon::Graphic::Bounds _rotator{};
		
		bool _dragging = false;
		bool _draggingScalar = false;
		bool _draggingRotate = false;

		Tristeon::Vector _draggingOffset{};
	};

	SCENE_EDITOR(Tristeon::ActorLayer, ActorLayerSceneView);
}