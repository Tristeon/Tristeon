#pragma once
#ifdef TRISTEON_EDITOR
#include "EditorWindow.h"
namespace TristeonEditor
{
	class Editor
	{
	public:
		void initialize();
		void addWindow(EditorWindow* window);

		Tristeon::Layer* selectedLayer() const;
		void selectedLayer(Tristeon::Layer* value);
	private:
		Tristeon::Layer* _selectedLayer = nullptr;
		Tristeon::Vector<EditorWindow*> windows;
	};
}
#endif