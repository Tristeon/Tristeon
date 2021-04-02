#include "Scenes/Layers/Layer.h"
#include "LayerEditor.h"
#include "Editor.h"
#include <Editor/Dynamic/EditorRegister.h>

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
			layout->removeWidget(current->widget());
			current.reset();
		}
		
		if (layer == nullptr)
			return;

		auto serialized = layer->serialize();
		current = EditorRegister::createInstance(serialized["typeID"], serialized, [=](nlohmann::json value) { layer->deserialize(value); });
		if (current != nullptr)
			layout->addWidget(current->widget());
	}
}