#pragma once
#include "MenuBar.h"
#ifdef TRISTEON_EDITOR
#include "EditorWindow.h"

#include <Actors/Actor.h>
namespace TristeonEditor
{
	class Editor
	{
	public:
		void initialize();
		void addWindow(EditorWindow* window);

		Tristeon::Layer* selectedLayer() const;
		void selectedLayer(Tristeon::Layer* value);

		Tristeon::Actor* selectedActor() const;
		void selectedActor(Tristeon::Actor* value);
		void setMenuBar(MenuBar* menu_bar);
	private:
		Tristeon::Layer* _selectedLayer = nullptr;
		Tristeon::Actor* _selectedActor = nullptr;
		MenuBar* menuBar = nullptr;
		Tristeon::Vector<EditorWindow*> windows;
	};
}
#endif