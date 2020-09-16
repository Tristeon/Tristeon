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
		 * Creates instance of an object that inherits from Base.
		 * The user must take ownership of the instance himself.
		 */
		[[nodiscard]] static Unique<Base> createInstance(const std::string& s)
		{
			const auto it = getMap()->find(s);
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