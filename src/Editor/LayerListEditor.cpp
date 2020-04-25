#ifdef TRISTEON_EDITOR
#include "LayerListEditor.h"

#include <Editor/Editor.h>

#include <iostream>
#include <Scenes/Scene.h>
#include <Scenes/SceneManager.h>

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
		
		connect(list->model(), &QAbstractItemModel::rowsMoved, this, &LayerListEditor::rowsMoved);
		connect(list, &QListWidget::currentItemChanged, this, &LayerListEditor::selectionChanged);
		connect(list, &QListWidget::itemChanged, this, &LayerListEditor::itemRenamed);
	}

	void LayerListEditor::sceneLoaded(Tristeon::Scene* scene)
	{
		list->clear();
		layers.clear();

		for (size_t i = 0; i < scene->getLayerCount(); i++)
		{
			Tristeon::Layer* layer = scene->getLayer(i);

			QListWidgetItem* item = new QListWidgetItem(layer->name.c_str());
			item->setFlags(item->flags() | Qt::ItemIsEditable);
			list->addItem(item);
			layers[list->item(i)] = layer;
		}
	}

	void LayerListEditor::addButtonPressed()
	{
		QMenu contextMenu(tr("Context menu"), this);

		QAction action1("ActorLayer", this);
		connect(&action1, &QAction::triggered, this, [&](bool checked)
			{
				auto* item = new QListWidgetItem(QString("New ActorLayer"));
				item->setFlags(item->flags() | Qt::ItemIsEditable);
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
				item->setFlags(item->flags() | Qt::ItemIsEditable);
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

	void LayerListEditor::rowsMoved(const QModelIndex& parent, int start, int end, const QModelIndex& destination, int row)
	{
		for (int i = 0; i < list->count(); i++)
			Tristeon::SceneManager::current()->setIndex(layers[list->item(i)], i);
	}

	void LayerListEditor::selectionChanged(QListWidgetItem* current, QListWidgetItem* previous)
	{
		editor->selectedLayer(layers[current]);
	}

	void LayerListEditor::itemRenamed(QListWidgetItem* item)
	{
		layers[item]->name = item->text().toStdString();
	}
}
#endif