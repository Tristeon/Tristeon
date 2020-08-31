#pragma once
#ifdef TRISTEON_EDITOR
#include "Editor/Dynamic/AbstractEditor.h"
#include "FileItem.h"
#include <QtWidgets>

namespace TristeonEditor
{
	class FileEditor : public AbstractEditor
	{
	public:
		FileEditor()
		{
			layout = new QVBoxLayout(this);
			layout->setAlignment(Qt::AlignTop);
			setLayout(layout);
		}
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override
		{
			item = dynamic_cast<FileItem*>(current);

			if (item == nullptr)
				throw std::exception("FileEditor's target must be of type FileItem!");
		}
	protected:
		FileItem* item = nullptr;
		QVBoxLayout* layout = nullptr;
	};
}
#endif