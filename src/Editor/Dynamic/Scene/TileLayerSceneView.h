#pragma once
#include <QtWidgets>
#include "SceneEditor.h"
#include <Scenes/Layers/TileLayer.h>
#include <Editor/Dynamic/Scene/SceneEditorRegister.h>

namespace TristeonEditor
{
	class TileLayerSceneView : public SceneEditor
	{
	public:
		explicit TileLayerSceneView(Tristeon::Layer* pLayer);

		void updateView() override;

	protected:
		QLabel* highlight = nullptr;
		Tristeon::TileLayer* _tileLayer = nullptr;

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

	SCENE_EDITOR(Tristeon::TileLayer, TileLayerSceneView);
}