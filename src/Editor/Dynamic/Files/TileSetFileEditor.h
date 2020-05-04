#pragma once
#ifdef TRISTEON_EDITOR
#include "FileEditor.h"
#include "FileEditorRegister.h"

namespace TristeonEditor
{
	class TileSetFileEditor : FileEditor
	{
		FILE_EDITOR_H("tileset", TileSetFileEditor)
	public:
		void initialize() override;
	};
}
#endif