#ifdef TRISTEON_EDITOR
#include "PropertyWindow.h"
#include "Dynamic/Files/FileEditorRegister.h"
#include <Editor/Dynamic/Objects/Actors/ActorEditor.h>
#include <Editor/Dynamic/Objects/ObjectEditorRegister.h>
#include <Editor/Editor.h>
#include "Scenes/Layers/Layer.h"
#include "Scenes/Layers/TileLayer.h"

namespace TristeonEditor
{
	void PropertyWindow::initialize()
	{
		layout = new QGridLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		contents = findChild<QWidget*>("properties_contents");
		contents->setLayout(layout);
		contents->show();

		if (Editor::instance()->selectedLayer() != nullptr)
			selectedLayerChanged(Editor::instance()->selectedLayer());
	}

	void PropertyWindow::sceneLoaded(Tristeon::Scene * scene)
	{
		//Empty
	}

	void PropertyWindow::selectedActorChanged(Tristeon::Actor * actor)
	{
		if (current != nullptr)
		{
			layout->removeWidget(current);
			current->deleteLater();
		}
		current = nullptr;

		if (actor == nullptr)
			return;

		current = ObjectEditorRegister::createInstance(actor->serialize()["typeID"]);
		if (current == nullptr)
			current = new ActorEditor(); //Default editor for actors as fallback

		current->target(actor);
		current->setParent(contents);
		current->initialize();
		layout->addWidget(current);
		current->show();
	}

	void PropertyWindow::selectedLayerChanged(Tristeon::Layer * layer)
	{
		if (current != nullptr)
		{
			layout->removeWidget(current);
			current->deleteLater();
		}
		current = nullptr;
	}

	void PropertyWindow::selectedFilePathChanged(FileItem* item)
	{
		if (current != nullptr)
		{
			layout->removeWidget(current);
			current->deleteLater();
		}
		current = nullptr;

		current = FileEditorRegister::createInstance(item->extension);

		if (current != nullptr) 
		{
			current->target(item);
			current->setParent(contents);
			current->initialize();
			layout->addWidget(current);
			current->show();
		}
	}
}
#endif