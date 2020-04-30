#ifdef TRISTEON_EDITOR
#include "CustomEditor.h"

namespace TristeonEditor
{
	CustomEditor::CustomEditor()
	{
		layout = new QVBoxLayout(this);
		layout->setAlignment(Qt::AlignTop);
		setLayout(layout);
	}

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

	Editor* CustomEditor::editor() const
	{
		return _editor;
	}

	void CustomEditor::editor(Editor* value)
	{
		_editor = value;
	}
}
#endif