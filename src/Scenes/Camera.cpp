#include "Camera.h"

#include <Scenes/Scene.h>
#include <Scenes/SceneManager.h>

namespace Tristeon
{
	REGISTER_TYPE_CPP(Camera)
	
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
		position = j["position"];
		size = j["size"];
		zoom = j["zoom"];
	}

	Camera* Camera::main()
	{
		if (SceneManager::current() == nullptr)
			return nullptr;
		
		return SceneManager::current()->getCamera();
	}
}
