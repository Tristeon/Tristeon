#pragma once
#ifdef TRISTEON_EDITOR
#include <QtWidgets>
#include <Editor/Dynamic/Objects/ObjectEditor.h>
#include <Editor/Dynamic/Objects/ObjectEditorRegister.h>
#include <Scenes/Layers/TileLayer.h>

namespace TristeonEditor
{
	class TileLayerEditor : public ObjectEditor
	{
	public:
		void initialize() override;
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;

	private:
		void mapColumnsChanged(int columns);
		void mapRowsChanged(int rows);
		void resizeMap(int columns, int rows);
		
		Tristeon::TileLayer* targetLayer = nullptr;
	};

	OBJECT_EDITOR(Tristeon::TileLayer, TileLayerEditor);
}
#endif