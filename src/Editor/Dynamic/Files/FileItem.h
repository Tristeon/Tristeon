#pragma once
#include <TObject.h>
#include <Standard/String.h>

namespace TristeonEditor
{
	struct FileItem : Tristeon::TObject
	{
		Tristeon::String name;
		Tristeon::String path;
		Tristeon::String globalPath;
		Tristeon::String extension;
	};
}