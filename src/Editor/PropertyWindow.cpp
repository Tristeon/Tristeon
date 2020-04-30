#ifdef TRISTEON_EDITOR
#include "PropertyWindow.h"
#include <Editor/Editor.h>
#include <Editor/CustomEditors/EditorRegister.h>
#include <Editor/CustomEditors/Actors/ActorEditor.h>

namespace TristeonEditor
{
	void PropertyWindow::initialize()
	{
		layout = new QGridLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		contents = findChild<QWidget*>("properties_contents");
		contents->setLayout(layout);
		contents->show();

		if (editor->selectedLayer() != nullptr)
			selectedLayerChanged(editor->selectedLayer());
	}

	void PropertyWindow::sceneLoaded(Tristeon::Scene * scene)
	{

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

		current = EditorRegister::createInstance(actor->serialize()["typeID"]);
		if (current == nullptr)
			current = new ActorEditor(); //Default editor for actors as fallback

		current->editor(editor);
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
}
#endif