#pragma once
#include <qwidget.h>
#include <Utils/ClassDefaults.h>

namespace TristeonEditor
{
	class AbstractEditor
	{
	public:
		AbstractEditor() = default;
		virtual ~AbstractEditor() { delete _widget; }
		virtual QWidget* widget() { return _widget; }

		DELETE_COPY(AbstractEditor);
		DEFAULT_MOVE(AbstractEditor);

	protected:
		QWidget* _widget = nullptr;
	};
}