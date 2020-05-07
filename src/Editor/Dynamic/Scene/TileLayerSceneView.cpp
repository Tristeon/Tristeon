#ifdef TRISTEON_EDITOR
#include "TileLayerSceneView.h"

namespace TristeonEditor
{
	SCENE_EDITOR_CPP(Tristeon::TileLayer, TileLayerSceneView);
	
	void TileLayerSceneView::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		SceneEditor::targetChanged(current, old);
		tileLayer = (Tristeon::TileLayer*)current;
	}

	void TileLayerSceneView::initialize()
	{
		//highlight = new QLabel(this);
		//highlight->setPixmap(QPixmap("Internal/Textures/selection.png"));
		//highlight->setScaledContents(true);
		//highlight->setAttribute(Qt::WA_TranslucentBackground);

		//Tristeon::Vector2 const size = tileLayer->set()->tileSizeNormalized() * Tristeon::Vector2(width(), height());
		//highlight->setMinimumSize((int)size.x, (int)size.y);
		//highlight->setMaximumSize((int)size.x, (int)size.y);
		//highlight->adjustSize();
		//highlight->show();
	}
}
#endif