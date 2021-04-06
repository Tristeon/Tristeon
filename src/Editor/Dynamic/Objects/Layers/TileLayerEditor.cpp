#include "Editor/EditorFields.h"
#include "TileLayerEditor.h"
#include <Serialization/JsonSerializer.h>

using Tristeon::Vector;
using Tristeon::VectorI;

namespace TristeonEditor
{
	TileLayerEditor::TileLayerEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback) : AbstractJsonEditor(pValue, pCallback)
	{
		auto* form = new QWidget();
		_widget = form;

		auto* formLayout = new QFormLayout();
		form->setLayout(formLayout);

		columns = std::make_unique<UIntEditor>(pValue["columns"], [=](nlohmann::json val) { mapColumnsChanged(val); });
		formLayout->addRow("Columns", columns->widget());
		rows = std::make_unique<UIntEditor>(pValue["rows"], [=](nlohmann::json val) { mapRowsChanged(val); });
		formLayout->addRow("Rows", rows->widget());
	}

	void TileLayerEditor::setValue(const nlohmann::json& pValue)
	{
		//TODO
	}

	void TileLayerEditor::mapColumnsChanged(int columns)
	{
		resizeMap(columns, _value["rows"]);
	}

	void TileLayerEditor::mapRowsChanged(int rows)
	{
		resizeMap(_value["columns"], rows);
	}

	void TileLayerEditor::resizeMap(int columns, int rows)
	{
		Tristeon::String data;
		_value["columns"] = columns;
		_value["rows"] = rows;

		//TODO: Copy map over after resizing
		for (unsigned int i = 0; i < columns * rows; i++)
		{
			data += std::to_string(-1) + "," + std::to_string(-1);
			if (i < columns * rows - 1)
				data += ",";
		}
		_value["tileData"] = data;
		_callback(_value);
	}
}