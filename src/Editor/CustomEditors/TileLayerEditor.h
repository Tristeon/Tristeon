#pragma once
#ifdef TRISTEON_EDITOR
#include <QtWidgets>
#include <Editor/CustomEditors/CustomEditor.h>
#include <Editor/CustomEditors/EditorRegister.h>
#include <Scenes/Layers/TileLayer.h>

namespace TristeonEditor
{
	class TileLayerEditor : public CustomEditor
	{
		CUSTOM_EDITOR_H(Tristeon::TileLayer, TileLayerEditor)
	public:
		void initialize() override;
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;

	Q_SLOTS
		void mapWidthChanged(int width);
		void mapHeightChanged(int height);
		void browse();

	protected:
		void mousePressEvent(QMouseEvent* event) override;

	private:
		void resizeMap(int width, int height);
		void loadTileset(Tristeon::TileSet* set);
		
		Tristeon::TileLayer* targetLayer = nullptr;
		
		QLabel* name = nullptr;
		QPushButton* button = nullptr;
		QLabel* image = nullptr;

		QLabel* tileHighlight = nullptr;
		int selectedTile = -1;
	};
}
#endif