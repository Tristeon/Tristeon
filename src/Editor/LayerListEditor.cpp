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
		connect(list, &QListWidget::itemClicked, this, [=](QListWidgetItem* item) { Editor::instance()->selectedLayer(layers[item]); });
		connect(list, &QListWidget::itemChanged, this, &LayerListEditor::itemRenamed);
	}

	void LayerListEditor::sceneLoaded(Tristeon::Scene* scene)
	{
		int const row = list->currentRow();
		list->clear();
		layers.clear();

		for (size_t i = 0; i < scene->layerCount(); i++)
		{
			Tristeon::Layer* layer = scene->layerAt(i);

			QListWidgetItem* item = new QListWidgetItem(layer->name.c_str());
			item->setFlags(item->flags() | Qt::ItemIsEditable);
			list->addItem(item);
			layers[list->item(i)] = layer;
		}

		if (row < scene->layerCount())
			list->setCurrentRow(row);
		else
			list->setCurrentRow(0);
	}

	void LayerListEditor::addButtonPressed()
	{
		QMenu contextMenu(tr("Context menu"));

		auto* map = Tristeon::Register<Tristeon::Layer>::getMap();
		for (auto const& pair : *map)
		{
			QAction* action = new QAction(pair.first.c_str(), this);
			connect(action, &QAction::triggered, this, [&](bool checked)
				{
					auto* layer = Tristeon::SceneManager::current()->createLayer(pair.first);
					layer->name = "New " + Tristeon::StringHelper::split(pair.first, ':').last();

					auto* item = new QListWidgetItem(QString::fromStdString(layer->name));
					item->setFlags(item->flags() | Qt::ItemIsEditable);
					list->addItem(item);

					layers[item] = layer;

					list->setCurrentRow(list->count() - 1);
				});
			contextMenu.addAction(action);
		}
		
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
		Editor::instance()->selectedLayer(layers[current]);
	}

	void LayerListEditor::itemRenamed(QListWidgetItem* item)
	{
		layers[item]->name = item->text().toStdString();
	}
}
#endif