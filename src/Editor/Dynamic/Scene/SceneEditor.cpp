#ifdef TRISTEON_EDITOR
#include "SceneEditor.h"

namespace TristeonEditor
{
	void SceneEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		currentLayer = (Tristeon::Layer*)current;
	}
}
#endif