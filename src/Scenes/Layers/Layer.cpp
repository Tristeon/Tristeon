#include "Layer.h"

namespace Tristeon
{
	json Layer::serialize()
	{
		json j;
		j["name"] = name;
		return j;
	}

	void Layer::deserialize(json j)
	{
		name = j.value("name", "");
	}
}
