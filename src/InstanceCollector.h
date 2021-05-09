#pragma once
#include <map>
#include <Serialization/InstancedSerializable.h>

namespace Tristeon
{
	/**
	 * InstanceCollector collects all objects with an InstanceID.
	 * It behaves in a very similar way to Collector<T>, except that it only stores InstancedSerializable objects by instanceID and uses the fast lookup of std::map to find them back.
	 *
	 * InstancedSerializable automatically adds and removes each instance to the collector. 
	 * 
	 * InstanceCollector is used for application wide reference lookups for things like finding actors, layers, behaviours, etc.
	 */
	class InstanceCollector
	{
		friend InstancedSerializable;
		
	public:
		/**
		 * @brief Returns the object with the given ID, usually obtained through json data or through InstancedSerializable::instanceID().
		 *
		 * The function is most notably used in scene references, Actor::find() and SerializedInstance::get(), but there are other manual use cases where this function can be particularly convenient.
		 *
		 * Since instancedSerializables set their instanceID upon deserialize(), you can not use InstancedSerializable in the deserialization phase of scene loading.
		 * 
		 * @return Returns the InstancedSerializable, or nullptr if no fitting InstancedSerializable exists.
		 */
		static InstancedSerializable* find(const unsigned int& id);

	private:
		/**
		 * Adds an object to the collector.
		 *
		 * In debug mode, this function throws if the item is already in the collector
		 */
		static void add(InstancedSerializable* t);

		/**
		 * Removes the object from the collector.
		 *
		 * If the item isn't in the list, this function does nothing.
		 */
		static void remove(InstancedSerializable* t);
		
		static std::map<unsigned int, InstancedSerializable*> _collection;
	};
}