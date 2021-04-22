#pragma once
#include <qopenglwidget.h>
#include <Standard/String.h>

namespace TristeonEditor
{
	class AssetPreview : public QWidget
	{
	public:
		AssetPreview(const Tristeon::String& path) : _path(path)
		{

		}

	protected:
		Tristeon::String _path;
	};
}