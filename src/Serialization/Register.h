#pragma once
#include <map>
#include <Utils/TypeDefinitions.h>

namespace Tristeon
{
	template <typename T, typename Base>
	[[nodiscard]] Unique<Base> CreateInstance() { return std::make_unique<T>(); }

	/**
	 * The register is a map that is used to record & create instances of registered types
	 * In order to create instances you can call createInstance()
	 */
	template <typename Base>
	struct Register
	{
		using TypeMap = std::map<String, Unique<Base>(*)()>;

		/**
		 * Creates an instance of the class that matches the given type name.
		 * In the case that the given type name string isn't found in the Register's type map, this function returns nullptr.
		 * 
		 * The function returns a unique ptr, meaning that the receiver must claim full ownership of the object.
		 */
		[[nodiscard]] static Unique<Base> createInstance(const String& type)
		{
			const auto it = getMap()->find(type);
			if (it == getMap()->end())
				return nullptr;
			return it->second();
		}

		/**
		 * Returns the full register map.
		 */
		[[nodiscard]] static TypeMap* getMap()
		{
			static TypeMap instance;
			return &instance;
		}
	};
}