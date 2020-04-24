#ifdef TRISTEON_EDITOR
#include "CustomEditor.h"

namespace TristeonEditor
{
	Tristeon::TObject* CustomEditor::target() const
	{
		return _target;
	}

	void CustomEditor::target(Tristeon::TObject* value)
	{
		auto* old = _target;
		_target = value;
		targetChanged(_target, old);
	}
}
#endif