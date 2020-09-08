#pragma once
#include <Utils/StringHelper.h>
#include "Utils/Console.h"

namespace Tristeon
{
#if defined(_MSC_VER)
	/**
	 * Wrapper around typeid(T).name(), to enforce a consistent return style across all (supported) compilers.
	 *
	 * The MSVC version removes the "typename " word before the type.
	 */
	template<typename T>
	String getTypename()
	{
		const String name = typeid(T).name();
		Vector<String> vec = StringHelper::split(name, ' ');
		if (vec.size() < 2)
		{
			Console::warning("Failed to interpret MSVC typename. Returning direct typeid(T).name(): " + String(typeid(T).name()));
			return name;
		}

		std::string result;
		for (auto i = 1; i < vec.size(); i++)
		{
			result += vec[i];
		}
		return result;
	}
#else
#include <cxxabi.h>

	/**
	 * Wrapper around typeid(T).name(), to enforce a consistent return style across all (supported) compilers.
	 *
	 * The MinGW version demangles the typename
	 */
	template<typename T>
	std::string getTypename()
	{
		int status;
		char* realName = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
		std::string result = { realName };
		free(realName);
		return result;
	}
#endif
	
#define TRISTEON_TYPENAME(T) Tristeon::getTypename<T>()
	
}
