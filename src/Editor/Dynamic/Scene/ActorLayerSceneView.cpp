#ifdef TRISTEON_EDITOR
#include "ActorLayerSceneView.h"
#include <Input/Mouse.h>

using namespace Tristeon;
namespace TristeonEditor
{
	SCENE_EDITOR_CPP(Tristeon::ActorLayer, ActorLayerSceneView);
	
	void ActorLayerSceneView::initialize()
	{
		
	}

	void ActorLayerSceneView::updateView()
	{
		if (Mouse::pressed(Mouse::Left))
		{
		}
	}
}

#endif