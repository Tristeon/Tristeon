#pragma once
#ifdef TRISTEON_EDITOR
#include <TObject.h>
#include <Utils/TypeDefinitions.h>

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
#endif