#include "Graphic.h"

#include "Window.h"
#include <Input/Mouse.h>

#include "Engine.h"
#include "Renderer.h"
#include "Rendering/Camera.h"

namespace Tristeon
{
	bool Graphic::Bounds::contains(Vector const& pPosition) const
	{
		return pPosition.x > min.x && pPosition.x < max.x&& pPosition.y > min.y && pPosition.y < max.y;
	}

	Vector Graphic::Bounds::size() const
	{
		return (max - min);
	}

	bool Graphic::Bounds::underMouse() const
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
		j["renderMask"] = renderMask;
		return j;
	}

	void Graphic::deserialize(json pJson)
	{
		Actor::deserialize(pJson);
		display = pJson.value("display", true);
		renderMask = pJson.value("renderMask", RenderMask::Default);
	}
}