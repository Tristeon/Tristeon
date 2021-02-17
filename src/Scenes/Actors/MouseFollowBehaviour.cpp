#include "MouseFollowBehaviour.h"

#include "Window.h"
#include "Input/Mouse.h"
#include "Rendering/Camera.h"
#include "Serialization/Type.h"

namespace Tristeon
{
	json MouseFollowBehaviour::serialize()
	{
		json j = Behaviour::serialize();
		j["typeID"] = Type<MouseFollowBehaviour>::fullName();
		return j;
	}

	void MouseFollowBehaviour::deserialize(json j)
	{
		
	}

	void MouseFollowBehaviour::update()
	{
		actor()->position = Window::screenToWorld(Mouse::position(), Camera::cameras()[0]);
	}
}