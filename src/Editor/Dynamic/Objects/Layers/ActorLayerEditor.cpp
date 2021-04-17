#include "Editor/Palette.h"
#include "ActorLayerEditor.h"
#include <Editor/Editor.h>
#include <QtWidgets>
#include <InstanceCollector.h>

namespace TristeonEditor
{
	ActorLayerEditor::ActorLayerEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback) : AbstractJsonEditor(pValue, pCallback)
	{
		_widget = new QWidget();
		QVBoxLayout* layout = new QVBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		_widget->setLayout(layout);

		list = new QListWidget();
		list->setDragDropMode(QAbstractItemView::NoDragDrop);
		list->setDefaultDropAction(Qt::DropAction::IgnoreAction);
		list->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
		list->setAlternatingRowColors(true);
		list->setStyleSheet(Palette::getRGBString("alternate-background-color", Palette::background));
		layout->addWidget(list);

		QWidget::connect(list, &QListWidget::currentRowChanged, [=](int index) { selectedActorChanged(index); });
		QWidget::connect(list, &QListWidget::itemChanged, [=](QListWidgetItem* item) { actorRenamed(item); });
		QWidget::connect(list, &QListWidget::itemClicked, [=](QListWidgetItem* item) { Editor::instance()->selectedActor(actors[item]); });

		Tristeon::ActorLayer* layer = dynamic_cast<Tristeon::ActorLayer*>(Tristeon::InstanceCollector::find(_value["instanceID"]));
		if (layer == nullptr)
			return;
		for (size_t i = 0; i < layer->actorCount(); i++)
		{
			Tristeon::Actor* actor = layer->actorAt(i);
			auto* item = new QListWidgetItem(QString::fromStdString(actor->name));
			item->setFlags(item->flags() | Qt::ItemIsEditable);
			list->addItem(item);
			actors[item] = actor;
		}

		auto* buttonsParent = new QWidget();
		layout->addWidget(buttonsParent);

		auto* horizontal = new QHBoxLayout(buttonsParent);
		buttonsParent->setLayout(horizontal);

		auto* add = new QPushButton("+");
		add->setStyleSheet(Palette::getRGBString("background-color", Palette::add));
		horizontal->addWidget(add);
		QWidget::connect(add, &QPushButton::clicked, [=]() { addActor(); });

		auto* remove = new QPushButton("-");
		remove->setStyleSheet(Palette::getRGBString("background-color", Palette::remove));
		horizontal->addWidget(remove);
		QWidget::connect(remove, &QPushButton::clicked, [=]() { removeActor(); });

		nameChangedCallback = Editor::instance()->onSelectedActorNameChanged += [&](Tristeon::String const name)
		{
			if (list->currentItem())
				list->currentItem()->setText(!name.empty() ? QString::fromStdString(name) : "No Name");
		};
	}

	ActorLayerEditor::~ActorLayerEditor()
	{
		if (Editor::instance())
			Editor::instance()->onSelectedActorNameChanged -= nameChangedCallback;
	}

	void ActorLayerEditor::setValue(const nlohmann::json& pValue)
	{
		//TODO
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
		QMenu contextMenu(QWidget::tr("Context menu"));

		QAction* actor = new QAction(Tristeon::Type<Tristeon::Actor>::fullName().c_str());
		QWidget::connect(actor, &QAction::triggered, [&](bool checked)
			{
				Tristeon::ActorLayer* layer = dynamic_cast<Tristeon::ActorLayer*>(Tristeon::InstanceCollector::find(_value["instanceID"]));
				if (layer == nullptr)
					return;

				Tristeon::Actor* a = layer->createActor(Tristeon::Type<Tristeon::Actor>::fullName());
				a->name = "New Actor";

				auto* item = new QListWidgetItem(QString::fromStdString(a->name));
				item->setFlags(item->flags() | Qt::ItemIsEditable);
				list->addItem(item);

				actors[item] = a;

				list->setCurrentRow(list->count() - 1);
			});
		contextMenu.addAction(actor);
		
		auto* map = Tristeon::Register<Tristeon::Actor>::getMap();
		for (auto const& pair : *map)
		{
			QAction* action = new QAction(pair.first.c_str());
			QWidget::connect(action, &QAction::triggered, [&](bool checked)
				{
					Tristeon::ActorLayer* layer = dynamic_cast<Tristeon::ActorLayer*>(Tristeon::InstanceCollector::find(_value["instanceID"]));
					if (layer == nullptr)
						return;

					Tristeon::Actor* a = layer->createActor(pair.first);
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
		if (actor == nullptr)
			return;
		actor->destroy();

		actors.erase(list->currentItem());
		list->takeItem(list->currentRow());
	}
}