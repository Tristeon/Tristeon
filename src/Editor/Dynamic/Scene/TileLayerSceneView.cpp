#include "Input/Keyboard.h"
#ifdef TRISTEON_EDITOR
#include "Engine.h"
#include "Input/Mouse.h"
#include "Scenes/Camera.h"
#include "TileLayerSceneView.h"
#include "Editor/Brushes/Brushes.h"

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
		if (Engine::playMode())
		{
			highlight->hide();
			return;
		}
		highlight->show();

		if (Camera::main()->position != cameraPos || Camera::main()->zoom != cameraZoom)
		{
			cameraPos = Camera::main()->position;
			cameraZoom = Camera::main()->zoom;

			updateTileSize();
			updateTilePosition(lastMousePos);

		}
		drawTile(lastTileIndex);
	}

	void TileLayerSceneView::mouseMoveEvent(QMouseEvent* event)
	{
		SceneEditor::mouseMoveEvent(event);

		if (Engine::playMode())
			return;

		lastMousePos = Mouse::position();
		updateTilePosition(lastMousePos);
	}

	void TileLayerSceneView::mousePressEvent(QMouseEvent* event)
	{
		SceneEditor::mousePressEvent(event);

		if (Engine::playMode())
			return;

		drawTile(lastTileIndex);
	}

	void TileLayerSceneView::resizeEvent(QResizeEvent* event)
	{
		updateTileSize();
		updateTilePosition(lastMousePos);
	}

	void TileLayerSceneView::updateTileSize()
	{
		Vector2 const screenScale = Vector2(width(), height()) / Vector2(1920, 1080);
		Vector2 const size = Vector2(tileLayer->tileRenderWidth(), tileLayer->renderHeight()) * screenScale * Camera::main()->zoom;
		highlight->setMinimumSize((int)size.x, (int)size.y);
		highlight->setMaximumSize((int)size.x, (int)size.y);
		highlight->adjustSize();
	}

	void TileLayerSceneView::updateTilePosition(Vector2Int mousePos)
	{
		Camera* camera = Camera::main();
		Vector2 const scalar = Vector2{ width() / (float)camera->size.x, height() / (float)camera->size.y } *Camera::main()->zoom;
		Vector2 const cameraPos = (Vector2)Camera::main()->position * scalar;

		Vector2Int const tileIndex = tileLayer->indexByPosition(GameView::screenToWorld(mousePos));

		Vector2 position = { width() / 2.0f, height() / 2.0f }; //Start at center of the screen coz tiles start there too
		position -= Vector2{ tileLayer->tileRenderWidth() / 2.0f, tileLayer->renderHeight() / 2.0f } *scalar; //Adjust center 
		position += Vector2{ (float)tileLayer->tileRenderWidth() * tileIndex.x, (float)tileLayer->renderHeight() * -tileIndex.y } *scalar; //Adjust based on tile index

		highlight->move(position.x - cameraPos.x, position.y + cameraPos.y);
		highlight->show();

		lastTileIndex = tileIndex;
	}

	void TileLayerSceneView::drawTile(Tristeon::Vector2Int tileIndex)
	{
		if (!underMouse())
			return;
		
		if (Keyboard::held(Keyboard::Alt)) return;
		
		if ((Mouse::pressed(Mouse::Left) || Mouse::held(Mouse::Left)) && tileLayer->checkBoundsByIndex(tileIndex))
			Brushes::current()->draw(tileLayer, tileIndex);
		else if ((Mouse::pressed(Mouse::Right) || Mouse::held(Mouse::Right)) && tileLayer->checkBoundsByIndex(tileIndex))
			Brushes::current()->erase(tileLayer, tileIndex);
	}
}
#endif