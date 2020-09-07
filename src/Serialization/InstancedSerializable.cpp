#include "InstancedSerializable.h"
#include "Collectors/InstanceCollector.h"
#include "Utils/Random.h"

namespace Tristeon
{
	InstancedSerializable::InstancedSerializable()
	{
		InstanceCollector::add(this);
		_instanceID = Random::generateUInt();
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
		_instanceID = j.value("instanceID", _instanceID);
	}

	unsigned InstancedSerializable::instanceID() const
	{
		return _instanceID;
	}
}
