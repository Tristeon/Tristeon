#pragma once
#ifdef TRISTEON_EDITOR
#include <Editor/Dynamic/AbstractEditor.h>
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
			setLayout(layout);
		}
		virtual ~ObjectEditor() = default;
	protected:
		QVBoxLayout* layout = nullptr;
	};
}
#endif