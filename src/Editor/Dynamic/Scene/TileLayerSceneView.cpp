#ifdef TRISTEON_EDITOR
#include "Scenes/Camera.h"
#include "TileLayerSceneView.h"

using namespace Tristeon;

namespace TristeonEditor
{
	SCENE_EDITOR_CPP(Tristeon::TileLayer, TileLayerSceneView);
	
	void TileLayerSceneView::targetChanged(TObject* current, TObject* old)
	{
		SceneEditor::targetChanged(current, old);
		tileLayer = (TileLayer*)current;

		setMouseTracking(true);
	}

	void TileLayerSceneView::initialize()
	{
		highlight = new QLabel(this);
		highlight->setPixmap(QPixmap("Internal/Textures/selection.png"));
		highlight->setScaledContents(true);
		highlight->setAttribute(Qt::WA_TranslucentBackground);
		updateTileSize();
		updateTilePosition(lastMousePos);
		highlight->hide();
	}

	void TileLayerSceneView::updateView()
	{
		if (Camera::main()->position != cameraPos || Camera::main()->zoom != cameraZoom)
		{
			cameraPos = Camera::main()->position;
			cameraZoom = Camera::main()->zoom;

			updateTileSize();
			updateTilePosition(lastMousePos);
		}
	}

	void TileLayerSceneView::mouseMoveEvent(QMouseEvent* event)
	{
		lastMousePos = Vector2Int { event->pos().x(), event->pos().y() };
		updateTilePosition(lastMousePos);
	}

	void TileLayerSceneView::resizeEvent(QResizeEvent* event)
	{
		updateTileSize();
		updateTilePosition(lastMousePos);
	}

	void TileLayerSceneView::updateTileSize()
	{
		Vector2 const screenScale = Vector2(width(), height()) / Vector2(1920, 1080);
		Vector2 const size = Vector2(tileLayer->renderWidth(), tileLayer->renderHeight()) * screenScale * Camera::main()->zoom;
		highlight->setMinimumSize((int)size.x, (int)size.y);
		highlight->setMaximumSize((int)size.x, (int)size.y);
		highlight->adjustSize();
	}

	void TileLayerSceneView::updateTilePosition(Vector2Int mousePos)
	{
		mousePos.y = height() - mousePos.y;
		
		Vector2 scalar = Vector2{ width() / 1920.0f, height() / 1080.0f } *Camera::main()->zoom;
		Vector2 cameraPos = (Vector2)Camera::main()->position * scalar;

		Vector2Int tileIndex; //Hi future me don't doubt this math it works
		tileIndex.x = floor((mousePos.x - width() / 2.0f) / (tileLayer->renderWidth() * scalar.x)) + 7;
		tileIndex.y = floor((mousePos.y - height() / 2.0f + tileLayer->renderHeight()) / (tileLayer->renderHeight() * scalar.y)) - 4;

		Vector2 position = { width() / 2.0f, height() / 2.0f }; //Start at center of the screen coz tiles start there too
		position -= Vector2{ tileLayer->renderWidth() / 2.0f, tileLayer->renderHeight() / 2.0f } * scalar; //Adjust center 
		position += Vector2{ tileLayer->renderWidth() * (float)tileIndex.x, tileLayer->renderHeight() * -(float)tileIndex.y } *scalar; //Adjust based on tile index
		
		highlight->move(position.x - cameraPos.x, position.y + cameraPos.y);
		highlight->show();
	}
}
#endif