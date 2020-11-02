#pragma once
#include <map>
#include <Serialization/InstancedSerializable.h>

namespace Tristeon
{
	/**
	 * InstanceCollector collects all objects with an InstanceID.
	 * It behaves in a very similar way to Collector<T>, except that it stores items by instanceID and uses the fast look up of std::map to find them back.
	 * 
	 * InstanceCollector is used for application wide reference lookups.
	 */
	class InstanceCollector
	{
		friend InstancedSerializable;
		
	public:
		/**
		 * Returns the object with the given ID.
		 * Returns nullptr if no such object exists.
		 */
		static InstancedSerializable* find(const unsigned int& id);

	private:
		/**
		 * Adds an object to the collector.
		 */
		static void add(InstancedSerializable* t);

		/**
		 * Removes the object from the collector.
		 */
		static void remove(InstancedSerializable* t);
		
		static std::map<unsigned int, InstancedSerializable*> _collection;
	};
}