#include "Layer.h"

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
}