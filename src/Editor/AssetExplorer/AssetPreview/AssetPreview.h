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

		void setAnimating(const bool& pEnable) { _animate = pEnable; }

	protected:
		Tristeon::String _path;
		bool _animate = false;
	};
}