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
		
		EditorFields::uintField(form, "Width", targetLayer->width(), 1, std::numeric_limits<int>::max(), [&](int value) { mapWidthChanged(value); });
		EditorFields::uintField(form, "Height", targetLayer->height(), 1, std::numeric_limits<int>::max(), [&](int value) { mapHeightChanged(value); });
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

		int const oldWidth = j["width"];
		int const oldHeight = j["height"];
		json oldData = j["tiles"];
		
		json::array_t data = json::array_t(width * height);
		j["width"] = width;
		j["height"] = height;
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				int const index = y * width + x;

				if (x < oldWidth && y < oldHeight)
					data.at(index) = oldData[y * oldWidth + x];
				else
					data.at(index) = Tristeon::Tile{ -1, -1 };
			}
		}
		j["tiles"] = data;
		targetLayer->deserialize(j);
	}
}
#endif