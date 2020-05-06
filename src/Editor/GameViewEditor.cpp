#ifdef TRISTEON_EDITOR
#include "Dynamic/Scene/SceneEditorRegister.h"
#include "Scenes/Layers/TileLayer.h"
#include "GameViewEditor.h"

namespace TristeonEditor
{
	void GameViewEditor::initialize()
	{
		layout = new QGridLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		setLayout(layout);
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
			layerView->editor(editor);
			layerView->target(layer);
			layerView->setParent(this);
			layerView->initialize();
			layerView->setAttribute(Qt::WA_TranslucentBackground);
			layout->addWidget(layerView);
			layerView->show();
		}
	}
}
#endif