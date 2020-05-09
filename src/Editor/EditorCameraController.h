#pragma once
#ifdef TRISTEON_EDITOR
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
		Tristeon::Vector2Int dragMousePos;
	};
}
#endif