#include "InstancedSerializable.h"
#include "InstanceCollector.h"
#include "Utils/Random.h"

namespace Tristeon
{
	InstancedSerializable::InstancedSerializable()
	{
		_instanceID = Random::generateUInt();
		InstanceCollector::add(this);
	}

	InstancedSerializable::~InstancedSerializable() noexcept
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
		InstanceCollector::remove(this);
		_instanceID = j.value("instanceID", _instanceID);
		InstanceCollector::add(this);
	}

	unsigned InstancedSerializable::instanceID() const
	{
		return _instanceID;
	}
}
