#ifdef TRISTEON_EDITOR
#include "Rendering/Camera.h"
#include "Window.h"
#include "Input/Keyboard.h"
#include "Rendering/Grid.h"
#include "Engine.h"
#include "Input/Mouse.h"
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

		if ((VectorI)Renderer::editorCamera()->position != cameraPos || Renderer::editorCamera()->zoom != cameraZoom)
		{
			cameraPos = (VectorI)Renderer::editorCamera()->position;
			cameraZoom = Renderer::editorCamera()->zoom;

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
		Vector const screenScale = Vector(width(), height()) / Window::gameSize();
		Vector const size = Vector((float)Grid::tileWidth(), (float)Grid::tileHeight()) * screenScale * Renderer::editorCamera()->zoom;
		highlight->setMinimumSize((int)size.x, (int)size.y);
		highlight->setMaximumSize((int)size.x, (int)size.y);
		highlight->adjustSize();
	}

	void TileLayerSceneView::updateTilePosition(VectorU mousePos)
	{
		Vector const scalar = Vector{ width() / (float)Window::gameWidth(), height() / (float)Window::gameHeight() } *Renderer::editorCamera()->zoom;
		Vector const cameraPos = (Vector)(VectorI)Renderer::editorCamera()->position * scalar;

		VectorI const tileIndex = Grid::indexByPosition(Window::screenToWorld(mousePos, Renderer::editorCamera()));

		Vector position = { width() / 2.0f, height() / 2.0f }; //Start at center of the screen coz tiles start there too
		position -= Vector{ (float)Grid::tileWidth() / 2.0f, (float)Grid::tileHeight() / 2.0f } *scalar; //Adjust center 
		position += Vector{ (float)Grid::tileWidth() * tileIndex.x, (float)Grid::tileHeight() * -tileIndex.y } *scalar; //Adjust based on tile index

		highlight->move(position.x - cameraPos.x, position.y + cameraPos.y);
		highlight->show();

		lastTileIndex = tileIndex;
	}

	void TileLayerSceneView::drawTile(Tristeon::VectorI tileIndex)
	{
		if (!underMouse())
			return;
		
		if (Keyboard::held(Keyboard::Alt)) return;

		if (tileLayer->tileset(Brushes::selectedTile().tilesetID) == nullptr)
			tileLayer->addTileset(Brushes::selectedTilesetPath());

		if ((Mouse::pressed(Mouse::Left) || Mouse::held(Mouse::Left)) && tileLayer->checkBoundsByIndex(tileIndex))
			Brushes::current()->draw(tileLayer, tileIndex);
		else if ((Mouse::pressed(Mouse::Right) || Mouse::held(Mouse::Right)) && tileLayer->checkBoundsByIndex(tileIndex))
			Brushes::current()->erase(tileLayer, tileIndex);
	}
}
#endif