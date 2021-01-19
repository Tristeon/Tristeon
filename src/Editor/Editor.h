#pragma once
#ifdef TRISTEON_EDITOR
#include "AssetExplorer/Importer.h"
#include "Utils/Singleton.h"

#include <Editor/DockWidget.h>
#include <Editor/MenuBar.h>

#include <Standard/Delegate.h>
#include <Scenes/Actors/Actor.h>

#include "Callbacks/ISceneLoaded.h"

namespace TristeonEditor
{
	struct FileItem;

	class Editor : public Tristeon::Singleton<Editor>, public Tristeon::ISceneLoaded
	{
	public:
		void initialize();
		void addWidget(DockWidget* window);

		Tristeon::Layer* selectedLayer() const;
		void selectedLayer(Tristeon::Layer* value);

		Tristeon::Actor* selectedActor() const;
		void selectedActor(Tristeon::Actor* value);

		FileItem* selectedFileItem() const;
		void selectedFileItem(FileItem* value);

		Tristeon::Delegate<Tristeon::String> onSelectedActorNameChanged;
		
		void setMenuBar(MenuBar* menu_bar);

		void sceneLoaded(Tristeon::Scene* scene) override;
	private:
		Tristeon::Layer* _selectedLayer = nullptr;
		Tristeon::Actor* _selectedActor = nullptr;
		FileItem* _selectedFileItem = nullptr;
		MenuBar* menuBar = nullptr;
		Importer importer;
		Tristeon::List<DockWidget*> windows;
	};
}
#endif