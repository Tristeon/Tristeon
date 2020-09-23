#include "Layer.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"

namespace Tristeon
{
	json Layer::serialize()
	{
		json j = InstancedSerializable::serialize();
		j["name"] = name;
		return j;
	}

	void Layer::deserialize(json j)
	{
		InstancedSerializable::deserialize(j);
		name = j.value("name", "");
	}

	void Layer::destroy()
	{
		SceneManager::current()->destroyLayer(this);
	}
}
