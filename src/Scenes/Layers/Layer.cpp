#include "Layer.h"

namespace Tristeon
{
	json Layer::serialize()
	{
		json j = Serializable::serialize();
		j["name"] = name;
		return j;
	}

	void Layer::deserialize(json j)
	{
		Serializable::deserialize(j);
		name = j.value("name", "");
	}
}