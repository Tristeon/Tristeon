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
		 * Get the instance pointer, the first time when this is called it performs a lookup through the InstanceCollector.
		 */
		T* get()
		{
			if (_instance == nullptr && !_invalid)
			{
				_instance = dynamic_cast<T*>(InstanceCollector::find(_id));
				_invalid = !_instance;
			}
			return _instance;
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
			_instance = nullptr;
		}
	private:
		T* _instance = nullptr;
		bool _invalid = false;
		unsigned int _id = 0;
	};
}