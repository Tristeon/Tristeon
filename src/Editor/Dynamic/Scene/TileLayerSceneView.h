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
		void updateView() override;

	protected:
		QLabel* highlight = nullptr;
		Tristeon::TileLayer* tileLayer = nullptr;

		float cameraZoom = 1.0f;
		Tristeon::VectorI cameraPos{};
		Tristeon::VectorU lastMousePos{};
		Tristeon::VectorI lastTileIndex{};
		
		void mouseMoveEvent(QMouseEvent* event) override;
		void mousePressEvent(QMouseEvent* event) override;
		void resizeEvent(QResizeEvent* event) override;
		void updateTileSize();
		void updateTilePosition(Tristeon::VectorU mousePos);

	private:
		void drawTile(Tristeon::VectorI tileIndex);
	};
}
#endif