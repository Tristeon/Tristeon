#include "Input/Keyboard.h"
#include "Math/Math.h"
#ifdef TRISTEON_EDITOR
#include "Input/Mouse.h"
#include "Scenes/Camera.h"
#include "EditorCameraController.h"

using namespace Tristeon;

namespace TristeonEditor
{
	void EditorCameraController::update()
	{
		if (Camera::main() == nullptr)
			return;
		
		handleScrolling();
		handleDragging();

		if (Keyboard::pressed(Keyboard::F) && Editor::instance()->selectedActor() != nullptr)
			Camera::main()->position = (Vector2Int)Editor::instance()->selectedActor()->position;
	}

	void EditorCameraController::handleDragging()
	{
		if (!dragging)
			dragMousePos = Mouse::position();
		dragging = Mouse::held(Mouse::Middle) || (Mouse::held(Mouse::Left) && Keyboard::held(Keyboard::Alt));

		if (dragging)
		{
			Vector2Int delta = Mouse::position() - dragMousePos;
			delta.y *= -1;
			delta.x *= -1;
			delta *= 3;
			delta *= 1.0f / Camera::main()->zoom;

			Camera::main()->position += delta;
			dragMousePos = Mouse::position();
		}
	}

	void EditorCameraController::handleScrolling()
	{
		float const scrollingSpeed = Math::clamp(-pow(Camera::main()->zoom - 1, 2) * 2 + 1, 0.1f, 10);
		Camera::main()->zoom += Mouse::deltaScroll().y * scrollingSpeed * 0.001f;
		Camera::main()->zoom = Math::clamp(Camera::main()->zoom, 0.03, 1.7);
	}
}
#endif