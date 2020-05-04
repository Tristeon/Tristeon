#pragma once
#ifdef TRISTEON_EDITOR
#include <json.h>
#include "FileEditor.h"
#include "FileEditorRegister.h"

namespace TristeonEditor
{
	class JsonFileEditor : public FileEditor
	{
		FILE_EDITOR_H("json", JsonFileEditor)
	public:
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
		void initialize() override;

		void displayJson(nlohmann::json& data, QWidget* parent, QFormLayout* form);
		void saveData();
	protected:
		nlohmann::json data;
	};
}
#endif