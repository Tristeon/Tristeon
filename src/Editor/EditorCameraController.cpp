#include "Math/Math.h"
#ifdef TRISTEON_EDITOR
#include "Input/Mouse.h"
#include "Scenes/Camera.h"
#include "EditorCameraController.h"

namespace TristeonEditor
{
	void EditorCameraController::update()
	{
		if (Tristeon::Camera::main() == nullptr)
			return;
		
		handleScrolling();
		handleDragging();
	}

	void EditorCameraController::handleDragging()
	{
		if (!dragging)
			dragMousePos = Tristeon::Mouse::position();
		dragging = Tristeon::Mouse::held(Tristeon::Mouse::Middle);

		if (dragging)
		{
			Tristeon::Vector2Int delta = Tristeon::Mouse::position() - dragMousePos;
			delta.y *= -1;
			delta.x *= -1;
			delta *= 3;
			delta *= 1.0f / Tristeon::Camera::main()->zoom;

			Tristeon::Camera::main()->position += delta;
			dragMousePos = Tristeon::Mouse::position();
		}
	}

	void EditorCameraController::handleScrolling()
	{
		float const scrollingSpeed = Tristeon::Math::clamp(-pow(Tristeon::Camera::main()->zoom - 1, 2) * 2 + 1, 0.1f, 10);
		Tristeon::Camera::main()->zoom += Tristeon::Mouse::deltaScroll().y * scrollingSpeed * 0.001f;
		Tristeon::Camera::main()->zoom = Tristeon::Math::clamp(Tristeon::Camera::main()->zoom, 0.03, 1.7);
	}
}
#endif