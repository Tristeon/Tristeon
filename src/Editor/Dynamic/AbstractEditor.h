#pragma once
#include <qwidget.h>
#include <Utils/ClassDefaults.h>

namespace TristeonEditor
{
	class AbstractEditor
	{
	public:
		AbstractEditor() = default;
		virtual ~AbstractEditor() = default;
		virtual QWidget* widget() = 0;

		DELETE_COPY(AbstractEditor);
		DEFAULT_MOVE(AbstractEditor);
	};
}