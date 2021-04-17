#include "Rendering/Camera.h"
#include "Window.h"
#include "Input/Keyboard.h"
#include "Rendering/Grid.h"
#include "Engine.h"
#include "Input/Mouse.h"
#include "TileLayerSceneView.h"
#include "Editor/Brushes/Brushes.h"

namespace TristeonEditor
{
	TileLayerSceneView::TileLayerSceneView(Tristeon::Layer* pLayer): SceneEditor(pLayer), _tileLayer(dynamic_cast<Tristeon::TileLayer*>(pLayer))
	{
		setMouseTracking(true);

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
		if (Tristeon::Engine::playMode())
		{
			highlight->hide();
			return;
		}
		highlight->show();

		if ((Tristeon::VectorI)Tristeon::Renderer::editorCamera()->position != cameraPos || Tristeon::Renderer::editorCamera()->zoom != cameraZoom)
		{
			cameraPos = (Tristeon::VectorI)Tristeon::Renderer::editorCamera()->position;
			cameraZoom = Tristeon::Renderer::editorCamera()->zoom;

			updateTileSize();
			updateTilePosition(lastMousePos);
		}
		drawTile(lastTileIndex);
	}

	void TileLayerSceneView::mouseMoveEvent(QMouseEvent* event)
	{
		SceneEditor::mouseMoveEvent(event);

		if (Tristeon::Engine::playMode())
			return;

		lastMousePos = Tristeon::Mouse::position();
		updateTilePosition(lastMousePos);
	}

	void TileLayerSceneView::mousePressEvent(QMouseEvent* event)
	{
		SceneEditor::mousePressEvent(event);

		if (Tristeon::Engine::playMode())
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
		Tristeon::Vector const screenScale = Tristeon::Vector(width(), height()) / Tristeon::Window::gameSize();
		Tristeon::Vector const size = Tristeon::Vector((float)Tristeon::Grid::tileWidth(), (float)Tristeon::Grid::tileHeight()) * screenScale * Tristeon::Renderer::editorCamera()->zoom;
		highlight->setMinimumSize((int)size.x, (int)size.y);
		highlight->setMaximumSize((int)size.x, (int)size.y);
		highlight->adjustSize();
	}

	void TileLayerSceneView::updateTilePosition(Tristeon::VectorU mousePos)
	{
		Tristeon::Vector const scalar = Tristeon::Vector{ width() / (float)Tristeon::Window::gameWidth(), height() / (float)Tristeon::Window::gameHeight() } * Tristeon::Renderer::editorCamera()->zoom;
		Tristeon::Vector const cameraPos = (Tristeon::Vector)(Tristeon::VectorI)Tristeon::Renderer::editorCamera()->position * scalar;

		Tristeon::VectorI const tileIndex = Tristeon::Grid::indexByPosition(Tristeon::Window::screenToWorld(mousePos, Tristeon::Renderer::editorCamera()));

		Tristeon::Vector position = { width() / 2.0f, height() / 2.0f }; //Start at center of the screen coz tiles start there too
		position -= Tristeon::Vector{ (float)Tristeon::Grid::tileWidth() / 2.0f, (float)Tristeon::Grid::tileHeight() / 2.0f } *scalar; //Adjust center 
		position += Tristeon::Vector{ (float)Tristeon::Grid::tileWidth() * tileIndex.x, (float)Tristeon::Grid::tileHeight() * -tileIndex.y } *scalar; //Adjust based on tile index

		highlight->move(position.x - cameraPos.x, position.y + cameraPos.y);
		highlight->show();

		lastTileIndex = tileIndex;
	}

	void TileLayerSceneView::drawTile(Tristeon::VectorI tileIndex)
	{
		if (!underMouse())
			return;
		
		if (Tristeon::Keyboard::held(Tristeon::Keyboard::Alt)) return;

		if (_tileLayer->tileset(Brushes::selectedTile().tilesetID) == nullptr)
			_tileLayer->addTileset(Brushes::selectedTilesetPath());

		if ((Tristeon::Mouse::pressed(Tristeon::Mouse::Left) || Tristeon::Mouse::held(Tristeon::Mouse::Left)) && _tileLayer->checkBoundsByIndex(tileIndex))
			Brushes::current()->draw(_tileLayer, tileIndex);
		else if ((Tristeon::Mouse::pressed(Tristeon::Mouse::Right) || Tristeon::Mouse::held(Tristeon::Mouse::Right)) && _tileLayer->checkBoundsByIndex(tileIndex))
			Brushes::current()->erase(_tileLayer, tileIndex);
	}
}