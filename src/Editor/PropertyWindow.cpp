#include "PropertyWindow.h"
#include <Editor/Editor.h>

#include <Editor/Dynamic/EditorRegister.h>
#include <Editor/Dynamic/FileEditorRegister.h>

#include <Editor/Dynamic/Objects/Actors/ActorEditor.h>
#include <Editor/Dynamic/Files/FileItem.h>

namespace TristeonEditor
{
	void PropertyWindow::initialize()
	{
		layout = new QGridLayout();
		contents = findChild<QWidget*>("properties_contents");
		contents->setLayout(layout);
		contents->show();

		selectedLayerChanged(Editor::instance()->selectedLayer());
	}

	void PropertyWindow::sceneLoaded(Tristeon::Scene * scene)
	{
		current.reset();
	}

	void PropertyWindow::selectedActorChanged(Tristeon::Actor * actor)
	{
		current.reset();

		if (actor == nullptr)
			return;

		auto serialized = actor->serialize();

		current = EditorRegister::createInstance(serialized["typeID"], serialized,
			[=](nlohmann::json pValue) { actor->deserialize(pValue); }
		);

		if (current == nullptr)
		{
			current = std::make_unique<ActorEditor>(serialized, 
				[=](nlohmann::json pValue) { actor->deserialize(pValue); }
			);
		}

		layout->addWidget(current->widget());
	}

	void PropertyWindow::selectedLayerChanged(Tristeon::Layer * layer)
	{
		current.reset();
	}

	void PropertyWindow::selectedFilePathChanged(FileItem* item)
	{
		current.reset();
		current = FileEditorRegister::createInstance(item->extension, *item);

		if (current != nullptr) 
			layout->addWidget(current->widget());
	}
}