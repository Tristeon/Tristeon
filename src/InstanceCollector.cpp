#include <InstanceCollector.h>

namespace Tristeon
{
	std::map<unsigned int, InstancedSerializable*> InstanceCollector::_collection{};

	void InstanceCollector::add(InstancedSerializable* t)
	{
		assert(_collection.find(t->instanceID()) == _collection.end());
		
		_collection[t->instanceID()] = t;
	}

	void InstanceCollector::remove(InstancedSerializable* t)
	{
		_collection.erase(t->instanceID());
	}

	InstancedSerializable* InstanceCollector::find(const unsigned& id)
	{
		if (_collection.find(id) == _collection.end())
			return nullptr;

		return _collection[id];
	}
}