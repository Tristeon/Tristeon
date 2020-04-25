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

		auto* buttonsParent = new QWidget(this);
		layout->addWidget(buttonsParent);

		auto* horizontal = new QHBoxLayout(buttonsParent);
		buttonsParent->setLayout(horizontal);
		
		auto* add = new QPushButton("+", this);
		add->setStyleSheet("background-color: rgb(0, 170, 0);");
		horizontal->addWidget(add);
		connect(add, &QPushButton::clicked, this, &ActorLayerEditor::addActor);
		
		auto* remove = new QPushButton("-", this);
		remove->setStyleSheet("background-color: rgb(170, 0, 0);");
		horizontal->addWidget(remove);
		connect(remove, &QPushButton::clicked, this, &ActorLayerEditor::removeActor);
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

	void ActorLayerEditor::addActor()
	{
		
	}

	void ActorLayerEditor::removeActor()
	{
		Tristeon::Actor* actor = actors[list->currentItem()];
		actor->destroy();

		actors.erase(list->currentItem());
		list->takeItem(list->currentRow());
	}
}
#endif