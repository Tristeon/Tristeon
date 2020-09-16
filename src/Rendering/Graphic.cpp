#include "Graphic.h"

#include "Window.h"
#include <Input/Mouse.h>

#include "Engine.h"
#include "Renderer.h"
#include "Rendering/Camera.h"

namespace Tristeon
{
	auto Graphic::Bounds::contains(Vector2 const& position) const -> bool
	{
		return position.x > min.x && position.x < max.x&& position.y > min.y && position.y < max.y;
	}

	Vector2 Graphic::Bounds::size() const
	{
		return (max - min);
	}

	bool Graphic::Bounds::underMouse()
	{
		if (Engine::playMode())
		{
			for (auto* camera : Camera::cameras())
			{
				if (contains(Window::screenToWorld(Mouse::position(), camera)))
					return true;
			}
		}

#ifdef TRISTEON_EDITOR
		if (!Engine::playMode() && contains(Window::screenToWorld(Mouse::position(), Renderer::editorCamera())))
			return true;
#endif
		
		return false;
	}

	json Graphic::serialize()
	{
		json j = Actor::serialize();
		j["display"] = display;
		return j;
	}

	void Graphic::deserialize(json j)
	{
		Actor::deserialize(j);
		display = j.value("display", true);
	}
}