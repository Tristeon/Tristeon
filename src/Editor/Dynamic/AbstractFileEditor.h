#pragma once
#include <Editor/Dynamic/AbstractEditor.h>
#include <Editor/Dynamic/Files/FileItem.h>

namespace TristeonEditor
{
	class AbstractFileEditor : public AbstractEditor
	{
	public:
		AbstractFileEditor(FileItem pItem) : _item(pItem) { }

	protected:
		FileItem _item;
	};
}