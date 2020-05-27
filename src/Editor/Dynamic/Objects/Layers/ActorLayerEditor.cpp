#ifdef TRISTEON_EDITOR
#include "ActorLayerEditor.h"
#include <Registers/ActorRegister.h>
#include <Editor/Editor.h>
#include <QtWidgets>

namespace TristeonEditor
{
	OBJECT_EDITOR_CPP(Tristeon::ActorLayer, ActorLayerEditor)

	void ActorLayerEditor::initialize()
	{
		list = new QListWidget(this);
		list->setDragDropMode(QAbstractItemView::NoDragDrop);
		list->setDefaultDropAction(Qt::DropAction::IgnoreAction);
		list->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
		list->setAlternatingRowColors(true);
		list->setStyleSheet("alternate-background-color: rgb(139, 139, 139);");
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

		nameChangedCallback = Editor::instance()->onSelectedActorNameChanged += [&](Tristeon::String const name)
		{
			list->currentItem()->setText(!name.empty() ? QString::fromStdString(name) : "No Name");
		};
	}

	ActorLayerEditor::~ActorLayerEditor()
	{
		if (Editor::instance())
			Editor::instance()->onSelectedActorNameChanged -= nameChangedCallback;
	}

	void ActorLayerEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		targetLayer = dynamic_cast<Tristeon::ActorLayer*>(current);
	}

	void ActorLayerEditor::selectedActorChanged(int index)
	{
		QListWidgetItem* item = list->item(index);
		Editor::instance()->selectedActor(actors[item]);
	}

	void ActorLayerEditor::actorRenamed(QListWidgetItem* item)
	{
		if (actors[item]->name == item->text().toStdString())
			return;
		
		actors[item]->name = item->text().toStdString();
		Editor::instance()->onSelectedActorNameChanged.invoke(item->text().toStdString());
	}

	void ActorLayerEditor::addActor()
	{
		QMenu contextMenu(tr("Context menu"));

		QAction* actor = new QAction(TRISTEON_TYPENAME(Tristeon::Actor).c_str(), this);
		connect(actor, &QAction::triggered, this, [&](bool checked)
			{
				Tristeon::Actor* a = targetLayer->createActor(TRISTEON_TYPENAME(Tristeon::Actor));
				a->name = "New Actor";

				auto* item = new QListWidgetItem(QString::fromStdString(a->name));
				item->setFlags(item->flags() | Qt::ItemIsEditable);
				list->addItem(item);

				actors[item] = a;

				list->setCurrentRow(list->count() - 1);
			});
		contextMenu.addAction(actor);
		
		auto* map = Tristeon::ActorRegister::getMap();
		for (auto const& pair : *map)
		{
			QAction* action = new QAction(pair.first.c_str(), this);
			connect(action, &QAction::triggered, this, [&](bool checked)
				{
					Tristeon::Actor* a = targetLayer->createActor(pair.first);
					a->name = "New " + Tristeon::StringHelper::split(pair.first, ':').last();

					auto* item = new QListWidgetItem(QString::fromStdString(a->name));
					item->setFlags(item->flags() | Qt::ItemIsEditable);
					list->addItem(item);

					actors[item] = a;
				});
			contextMenu.addAction(action);
		}
		
		contextMenu.exec(QCursor::pos());
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