#pragma once
#ifdef TRISTEON_EDITOR
#include "LayerListEditor.h"
namespace TristeonEditor
{
	class Editor
	{
	public:
		void initialize();
		Tristeon::Vector<EditorWindow*> windows;
	};
}
#endif