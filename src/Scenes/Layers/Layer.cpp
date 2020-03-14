#include "Layer.h"

namespace Tristeon
{
	void Layer::update()
	{
		//Empty
	}

	json Layer::serialize()
	{
		json j;
		j["name"] = name;
		return j;
	}

	void Layer::deserialize(json j)
	{
		name = j["name"].get<std::string>();
	}
}
