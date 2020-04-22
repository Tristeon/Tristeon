#ifdef TRISTEON_EDITOR
#include <Scenes/SceneManager.h>
#include "LayerListEditor.h"

#include <iostream>
#include <Scenes/Scene.h>

#include <Scenes/Layers/Layer.h>
#include <Scenes/Layers/ActorLayer.h>
#include <Scenes/Layers/TileLayer.h>

namespace TristeonEditor
{
	LayerListEditor::LayerListEditor(QWidget* parent) : QDockWidget(parent)
	{
		//Empty
	}

	void LayerListEditor::initialize()
	{
		auto* add = findChild<QPushButton*>(QString("add"));
		auto* remove = findChild<QPushButton*>(QString("remove"));
		list = findChild<QListWidget*>();

		connect(add, &QPushButton::pressed, this, &LayerListEditor::addButtonPressed);
		connect(remove, &QPushButton::pressed, this, &LayerListEditor::removeButtonPressed);
		connect(list, &QListWidget::indexesMoved, this, [&](const QModelIndexList & indexes) { std::cout << "Hello there, indices got changed" << std::endl; });
	}

	void LayerListEditor::loadScene(Tristeon::Scene* scene)
	{
		list->clear();
		layers.clear();

		for (size_t i = 0; i < scene->getLayerCount(); i++)
		{
			Tristeon::Layer* layer = scene->getLayer(i);
			list->addItem(QString(layer->name.c_str()));
			layers[list->item(i)] = layer;
		}
	}

	void LayerListEditor::addButtonPressed()
	{
		std::cout << "Hello there, I'd like to add a new layer" << std::endl;

		QMenu contextMenu(tr("Context menu"), this);

		QAction action1("ActorLayer", this);
		connect(&action1, &QAction::triggered, this, [&](bool checked)
			{
				auto* item = new QListWidgetItem(QString("New ActorLayer"));
				list->addItem(item);

				auto* layer = Tristeon::SceneManager::current()->addLayer<Tristeon::ActorLayer>();
				layer->name = "New ActorLayer";
				layers[item] = layer;
			});
		contextMenu.addAction(&action1);

		QAction action2("TileLayer", this);
		connect(&action2, &QAction::triggered, this, [&](bool checked)
			{
				auto* item = new QListWidgetItem(QString("New TileLayer"));
				list->addItem(item);

				auto* layer = Tristeon::SceneManager::current()->addLayer<Tristeon::TileLayer>();
				layer->name = "New TileLayer";
				layers[item] = layer;
			});
		contextMenu.addAction(&action2);
		
		contextMenu.exec(QCursor::pos());
	}

	void LayerListEditor::removeButtonPressed()
	{
		auto* remove = list->currentItem();
		if (remove == nullptr)
			return;

		Tristeon::SceneManager::current()->destroyLayer(layers[remove]);
		list->takeItem(list->currentRow());
		layers.erase(remove);
	}
}
#endif