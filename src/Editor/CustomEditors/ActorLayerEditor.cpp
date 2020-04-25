#ifdef TRISTEON_EDITOR
#include "ActorLayerEditor.h"
#include "Editor/Editor.h"
#include <QtWidgets>

namespace TristeonEditor
{
	CUSTOM_EDITOR_CPP(Tristeon::ActorLayer, ActorLayerEditor)

	void ActorLayerEditor::initialize()
	{
		list = new QListWidget(this);
		list->setDragDropMode(QAbstractItemView::NoDragDrop);
		list->setDefaultDropAction(Qt::DropAction::IgnoreAction);
		list->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
		list->setAlternatingRowColors(true);
		layout->addWidget(list);

		connect(list, &QListWidget::currentRowChanged, this, &ActorLayerEditor::selectedActorChanged);
		connect(list, &QListWidget::itemChanged, this, &ActorLayerEditor::actorRenamed);
		for (size_t i = 0; i < targetLayer->getActorCount(); i++)
		{
			Tristeon::Actor* actor = targetLayer->getActor(i);
			auto* item = new QListWidgetItem(QString::fromStdString(actor->name));
			item->setFlags(item->flags() | Qt::ItemIsEditable);
			list->addItem(item);
			actors[item] = actor;
		}
	}

	void ActorLayerEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		targetLayer = dynamic_cast<Tristeon::ActorLayer*>(current);
	}

	void ActorLayerEditor::selectedActorChanged(int index)
	{
		QListWidgetItem* item = list->item(index);
		editor()->selectedActor(actors[item]);
	}

	void ActorLayerEditor::actorRenamed(QListWidgetItem* item)
	{
		actors[item]->name = item->text().toStdString();
	}
}
#endif