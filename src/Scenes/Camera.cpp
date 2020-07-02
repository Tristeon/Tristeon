#include "Camera.h"

#include <Scenes/Scene.h>
#include <Scenes/SceneManager.h>

namespace Tristeon
{
	json Camera::serialize()
	{
		json j;
		j["typeID"] = TRISTEON_TYPENAME(Camera);
		j["position"] = position;
		j["size"] = size;
		j["zoom"] = zoom;
		return j;
	}

	void Camera::deserialize(json j)
	{
		position = j.value("position", Vector2Int::zero());
		size = j.value("size", Vector2Int::zero());
		zoom = j.value("zoom", 1.0f);
	}

	Camera* Camera::main()
	{
		if (SceneManager::current() == nullptr)
			return nullptr;
		
		return SceneManager::current()->getCamera();
	}
}
