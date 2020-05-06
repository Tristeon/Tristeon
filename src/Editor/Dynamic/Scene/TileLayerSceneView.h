#pragma once
#ifdef TRISTEON_EDITOR
#include <QtWidgets>
#include "SceneEditor.h"
#include "SceneEditorRegister.h"
#include <Scenes/Layers/TileLayer.h>
namespace TristeonEditor
{
	class TileLayerSceneView : public SceneEditor
	{
		SCENE_EDITOR_H(Tristeon::TileLayer, TileLayerSceneView);
	public:
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
		void initialize() override;

	protected:
		QLabel* highlight = nullptr;
		Tristeon::TileLayer* tileLayer = nullptr;
	};
}
#endif