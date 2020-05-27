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
		Tristeon::Vector2Int cameraPos{};
		Tristeon::Vector2Int lastMousePos{};
		Tristeon::Vector2Int lastTileIndex{};
		
		void mouseMoveEvent(QMouseEvent* event) override;
		void mousePressEvent(QMouseEvent* event) override;
		void resizeEvent(QResizeEvent* event) override;
		void updateTileSize();
		void updateTilePosition(Tristeon::Vector2Int mousePos);

	private:
		void drawTile(Tristeon::Vector2Int tileIndex);
	};
}
#endif