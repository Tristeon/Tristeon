#pragma once
#include <map>
#include <Serialization/InstancedSerializable.h>

namespace Tristeon
{
	/**
	 * InstanceCollector collects all objects with an InstanceID.
	 * This is used for application wide reference lookups.
	 */
	class InstanceCollector
	{
	public:
		/**
		 * Adds an object to the collector.
		 */
		static void add(InstancedSerializable* t)
		{
			collection[t->instanceID()] = t;
		}

		/**
		 * Removes the object from the collector.
		 */
		static void remove(InstancedSerializable* t)
		{
			collection.erase(t->instanceID());
		}

		static InstancedSerializable* find(const unsigned int& id)
		{
			if (collection.find(id) == collection.end())
				return nullptr;
			
			return collection[id];
		}

	private:
		static std::map<unsigned int, InstancedSerializable*> collection;
	};
}