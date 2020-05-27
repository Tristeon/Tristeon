#ifdef TRISTEON_EDITOR
#include "Scenes/Layers/Layer.h"
#include "LayerEditor.h"
#include "Editor.h"
#include "Dynamic/Objects/ObjectEditorRegister.h"

namespace TristeonEditor
{
	void LayerEditor::initialize()
	{
		layout = new QGridLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		contents = findChild<QWidget*>("contents");
		contents->setLayout(layout);
		contents->show();
		
		if (Editor::instance()->selectedLayer() != nullptr)
			selectedLayerChanged(Editor::instance()->selectedLayer());
	}

	void LayerEditor::sceneLoaded(Tristeon::Scene* scene)
	{
		selectedLayerChanged(nullptr);
	}

	void LayerEditor::selectedLayerChanged(Tristeon::Layer* layer)
	{
		if (current != nullptr)
		{
			layout->removeWidget(current);
			current->deleteLater();
		}
		current = nullptr;
		
		if (layer == nullptr)
			return;

		current = ObjectEditorRegister::createInstance(layer->serialize()["typeID"]);
		if (current != nullptr)
		{
			current->target(layer);
			current->setParent(contents);
			current->initialize();
			layout->addWidget(current);
			current->show();
		}
	}
}
#endif