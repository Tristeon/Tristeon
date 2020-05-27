#pragma once
#ifdef TRISTEON_EDITOR
#include "FileEditor.h"
#include "JsonFileEditor.h"
#include "FileEditorRegister.h"

namespace TristeonEditor
{
	class AnimationClipFileEditor : public JsonFileEditor
	{
	public:
		void initialize() override;
		void saveData() override;
	private:
		FILE_EDITOR_H("clip", AnimationClipFileEditor)
	};
}
#endif