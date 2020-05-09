#ifdef TRISTEON_EDITOR
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
		SceneEditor::mouseMoveEvent(event);
		
		lastMousePos = Vector2Int { event->pos().x(), event->pos().y() };
		updateTilePosition(lastMousePos);
	}

	void TileLayerSceneView::mousePressEvent(QMouseEvent* event)
	{
		SceneEditor::mousePressEvent(event);
		
		if (tileLayer->withinBounds(lastTileIndex))
			Brushes::current()->draw(tileLayer, (Vector2Int)lastTileIndex);
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
		
		Camera* camera = Camera::main();
		Vector2 const scalar = Vector2{ width() / (float)camera->size.x, height() / (float)camera->size.y } *Camera::main()->zoom;
		Vector2 cameraPos = (Vector2)Camera::main()->position * scalar;

		Vector2 tileIndex; //Don't doubt the math it works
		tileIndex.x = (-0.5f * width() / scalar.x + camera->position.x + 0.5 * tileLayer->renderWidth() + mousePos.x / scalar.x) / tileLayer->renderWidth();
		tileIndex.y = (-0.5f * height() / scalar.y + camera->position.y + 0.5 * tileLayer->renderHeight() + mousePos.y / scalar.y) / tileLayer->renderHeight();
		tileIndex = Vector2::floor(tileIndex);

		if ((Mouse::pressed(Mouse::Left) || Mouse::held(Mouse::Left)) && tileLayer->withinBounds(tileIndex))
			Brushes::current()->draw(tileLayer, (Vector2Int)tileIndex);
		
		Vector2 position = { width() / 2.0f, height() / 2.0f }; //Start at center of the screen coz tiles start there too
		position -= Vector2{ tileLayer->renderWidth() / 2.0f, tileLayer->renderHeight() / 2.0f } * scalar; //Adjust center 
		position += Vector2{ tileLayer->renderWidth() * tileIndex.x, tileLayer->renderHeight() * -tileIndex.y } *scalar; //Adjust based on tile index
		
		highlight->move(position.x - cameraPos.x, position.y + cameraPos.y);
		highlight->show();

		lastTileIndex = (Vector2Int)tileIndex;
	}
}
#endif