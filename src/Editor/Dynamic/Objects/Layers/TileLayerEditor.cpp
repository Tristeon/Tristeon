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
		
		EditorFields::uintField(form, "Columns", targetLayer->width(), 1, 1000, [&](int value) { mapWidthChanged(value); });
		EditorFields::uintField(form, "Rows", targetLayer->height(), 1, 1000, [&](int value) { mapHeightChanged(value); });
	}

	void TileLayerEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		targetLayer = dynamic_cast<Tristeon::TileLayer*>(current);
	}

	void TileLayerEditor::mapWidthChanged(int width)
	{
		resizeMap(width, targetLayer->height());
	}

	void TileLayerEditor::mapHeightChanged(int height)
	{
		resizeMap(targetLayer->width(), height);
	}

	void TileLayerEditor::resizeMap(int width, int height)
	{
		json j = targetLayer->serialize();

		int const oldWidth = j.value("width", 1);
		int const oldHeight = j.value("height", 1);
		json oldData = j.value("tiles", json());
		
		Tristeon::String data;
		j["width"] = width;
		j["height"] = height;
		for (unsigned int i = 0; i < width * height; i++)
		{
			data += std::to_string(-1) + "," + std::to_string(-1);
			if (i < width * height - 1)
				data += ",";
		}
		j["tileData"] = data;
		targetLayer->deserialize(j);
	}
}
#endif