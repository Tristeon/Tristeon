#pragma once
#include <QFileSystemModel>
#include <qlistwidget.h>

#include "TypeDefinitions.h"

namespace TristeonEditor
{
	class ContentViewer : public QListView
	{
	public:
		explicit ContentViewer(QWidget* parent);

		void setDirectory(QString directory);

	private:
		QFileSystemModel* model = nullptr;
	};
}
