#include "InstanceCollector.h"
#include "Utils/Random.h"

namespace Tristeon
{
	std::map<unsigned int, InstancedSerializable*> InstanceCollector::collection{};

	InstancedSerializable::InstancedSerializable()
	{
		InstanceCollector::add(this);
		_instanceID = Random::generateUInt();
	}

	InstancedSerializable::~InstancedSerializable()
	{
		InstanceCollector::remove(this);
	}

	json InstancedSerializable::serialize()
	{
		json j;
		j["instanceID"] = _instanceID;
		return j;
	}

	void InstancedSerializable::deserialize(json j)
	{
		_instanceID = j.value("instanceID", _instanceID);
	}
}