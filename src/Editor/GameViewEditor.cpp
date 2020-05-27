#ifdef TRISTEON_EDITOR
#include "Engine.h"
#include "Dynamic/Scene/SceneEditorRegister.h"
#include "Scenes/Layers/TileLayer.h"
#include "GameViewEditor.h"
#include "Rendering/Renderer.h"

namespace TristeonEditor
{
	void GameViewEditor::initialize()
	{
		Tristeon::Renderer::showGrid = true;

		layout = new QGridLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		setLayout(layout);

		cameraController = std::make_unique<EditorCameraController>();
	}

	void GameViewEditor::paintGL()
	{
		if (Tristeon::Engine::playMode() == false)
		{
			if (cameraController != nullptr)
				cameraController->update();
		}
		if (layerView != nullptr)
			layerView->updateView();

		GameView::paintGL();
	}

	void GameViewEditor::sceneLoaded(Tristeon::Scene* scene)
	{
	}

	void GameViewEditor::mouseMoveEvent(QMouseEvent* event)
	{
	}

	void GameViewEditor::selectedLayerChanged(Tristeon::Layer* layer)
	{
		if (layerView != nullptr)
		{
			layout->removeWidget(layerView);
			layerView->deleteLater();
			layerView = nullptr;
		}

		if (layer == nullptr)
			return;

		layerView = SceneEditorRegister::createInstance(layer->serialize()["typeID"]);
		if (layerView != nullptr)
		{
			layerView->target(layer);
			layerView->setParent(this);
			layerView->resize(width(), height());
			layerView->initialize();
			layerView->setAttribute(Qt::WA_TranslucentBackground);
			layout->addWidget(layerView);
			layerView->show();
		}
	}
}
#endif
