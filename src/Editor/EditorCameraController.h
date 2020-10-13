#pragma once
#ifdef TRISTEON_EDITOR

#include "Editor.h"
#include <Math/Vector.h>

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
		Tristeon::VectorI dragMousePos{};
	};
}
#endif