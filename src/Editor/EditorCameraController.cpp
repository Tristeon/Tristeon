#ifdef TRISTEON_EDITOR
#include "Rendering/Renderer.h"
#include "Rendering/Camera.h"
#include "Input/Keyboard.h"
#include "Math/Math.h"
#include "Input/Mouse.h"
#include "EditorCameraController.h"

using namespace Tristeon;

namespace TristeonEditor
{
	void EditorCameraController::update()
	{
		handleScrolling();
		handleDragging();

		if (Keyboard::pressed(Keyboard::F) && Editor::instance()->selectedActor() != nullptr)
			Renderer::editorCamera()->position = (VectorI)Editor::instance()->selectedActor()->position;
	}

	void EditorCameraController::handleDragging()
	{
		if (!dragging)
			dragMousePos = Mouse::position();
		dragging = Mouse::held(Mouse::Middle) || (Mouse::held(Mouse::Left) && Keyboard::held(Keyboard::Alt));

		if (dragging)
		{
			VectorI delta = Mouse::position() - dragMousePos;
			delta.y *= -1;
			delta.x *= -1;
			delta *= 3;
			delta *= 1.0f / Renderer::editorCamera()->zoom;

			Renderer::editorCamera()->position += delta;
			dragMousePos = Mouse::position();
		}
	}

	void EditorCameraController::handleScrolling()
	{
		if (!Renderer::editorCamera())
			return;
		
		float const scrollingSpeed = Math::clamp(-pow(Renderer::editorCamera()->zoom - 1, 2) * 2 + 1, 0.1f, 10.0f);
		Renderer::editorCamera()->zoom += Mouse::deltaScroll().y * scrollingSpeed * 0.001f;
		Renderer::editorCamera()->zoom = Math::clamp(Renderer::editorCamera()->zoom, 0.03, 1.7);
	}
}
#endif