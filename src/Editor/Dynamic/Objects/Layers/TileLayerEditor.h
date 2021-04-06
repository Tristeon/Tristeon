#pragma once
#include <Editor/Dynamic/AbstractJsonEditor.h>
#include <Editor/Dynamic/EditorRegister.h>

#include <Editor/Dynamic/Fields/UIntEditor.h>

#include <QtWidgets>
#include <Scenes/Layers/TileLayer.h>

namespace TristeonEditor
{
	class TileLayerEditor : public AbstractJsonEditor
	{
	public:
		TileLayerEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback);
		virtual void setValue(const nlohmann::json& pValue) override;

	private:
		void mapColumnsChanged(int columns);
		void mapRowsChanged(int rows);
		void resizeMap(int columns, int rows);
		
		Tristeon::Unique<UIntEditor> columns;
		Tristeon::Unique<UIntEditor> rows;
	};

	EDITOR(Tristeon::TileLayer, TileLayerEditor);
}