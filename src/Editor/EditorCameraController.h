#pragma once

#ifdef TRISTEON_EDITOR
#include "Editor.h"
#include <Math/Vector2Int.h>
namespace TristeonEditor
{
	class EditorCameraController
	{
	public:
		void update();
	private:
		void handleDragging();
		void handleScrolling();
		
		bool dragging = false;
		Tristeon::Vector2Int dragMousePos{};
	};
}
#endif