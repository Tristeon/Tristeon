#pragma once
#include <InstanceCollector.h>
#include <Serialization/Serializable.h>
#include <Serialization/Type.h>

namespace Tristeon
{
	/**
	 * InstanceReference is a way to store in-scene references to instanced serializables.
	 * The ID of the InstancedSerializable is stored and is then used in runtime to look up the instance, which can be acquired through get().
	 */
	template <typename T>
	struct InstanceReference : Serializable
	{
		static_assert(std::is_base_of<InstancedSerializable, T>::value, "InstanceReference<T>'s T has to inherit from InstancedSerializable");

		/**
		 * Get the instance pointer through a binary tree lookup.
		 */
		[[nodiscard]] T* get()
		{
			return dynamic_cast<T*>(InstanceCollector::find(_id));
		}

		/**
		 * Set the instance, this changes the id reference internally.
		 */
		void set(T* instance)
		{
			_id = instance->instanceID();
		}

		/**
		 * Sets the ID of the instance reference. If no instance with this id exists, get() returns nullptr.
		 */
		void set(const unsigned int& id)
		{
			_id = id;
		}
		
		json serialize() override
		{
			auto j = Serializable::serialize();
			j["typeID"] = Type<InstanceReference<T>>::fullName();
			j["id"] = _id;
			return j;
		}

		void deserialize(json j) override
		{
			Serializable::deserialize(j);
			_id = j.value("id", 0);
			_invalid = false;
		}
	private:
		bool _invalid = false;
		unsigned int _id = 0;
	};
}