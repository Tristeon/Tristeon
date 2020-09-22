#ifdef TRISTEON_EDITOR
#include "Editor/EditorFields.h"
#include "TileLayerEditor.h"
#include <Serialization/JsonSerializer.h>

using Tristeon::Vector2;
using Tristeon::Vector2Int;

namespace TristeonEditor
{
	OBJECT_EDITOR_CPP(Tristeon::TileLayer, TileLayerEditor)

	void TileLayerEditor::initialize()
	{
		setMouseTracking(true);
		
		auto* formParent = new QWidget(this);
		formParent->show();
		layout->addWidget(formParent);
		
		auto* form = new QFormLayout();
		formParent->setLayout(form);
		
		EditorFields::uintField(form, "Columns", targetLayer->columns(), 1, 1000, [&](int value) { mapColumnsChanged(value); });
		EditorFields::uintField(form, "Rows", targetLayer->rows(), 1, 1000, [&](int value) { mapRowsChanged(value); });
	}

	void TileLayerEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		targetLayer = dynamic_cast<Tristeon::TileLayer*>(current);
	}

	void TileLayerEditor::mapColumnsChanged(int columns)
	{
		resizeMap(columns, targetLayer->rows());
	}

	void TileLayerEditor::mapRowsChanged(int rows)
	{
		resizeMap(targetLayer->columns(), rows);
	}

	void TileLayerEditor::resizeMap(int columns, int rows)
	{
		json j = targetLayer->serialize();
		
		Tristeon::String data;
		j["columns"] = columns;
		j["rows"] = rows;

		//TODO: Copy map over after resizing
		for (unsigned int i = 0; i < columns * rows; i++)
		{
			data += std::to_string(-1) + "," + std::to_string(-1);
			if (i < columns * rows - 1)
				data += ",";
		}
		j["tileData"] = data;
		targetLayer->deserialize(j);
	}
}
#endif