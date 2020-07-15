#pragma once
#ifdef TRISTEON_EDITOR
#include "Editor/Dynamic/AbstractEditor.h"
#include <QWidget>
#include <QLayout>

namespace TristeonEditor
{
	class Editor;
	
	class ObjectEditor : public AbstractEditor
	{
	public:
		ObjectEditor()
		{
			layout = new QVBoxLayout(this);
			layout->setAlignment(Qt::AlignTop);
			layout->setContentsMargins(0, 0, 0, 0);
			setLayout(layout);
		}
		virtual ~ObjectEditor() = default;
	protected:
		QVBoxLayout* layout = nullptr;
	};
}
#endif