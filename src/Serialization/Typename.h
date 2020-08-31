#pragma once
#include <iostream>
#include <Utils/StringHelper.h>

namespace Tristeon
{
#if defined(_MSC_VER)
	/**
	 * Wrapper around typeid(T).name(), to enforce a consistent return style across all (supported) compilers.
	 */
	template<typename T>
	std::string getTypename()
	{
		String name = typeid(T).name();
		std::vector<String> vec = StringHelper::split(name, ' ');
		if (vec.size() < 2)
		{
			std::cout << "[Warning]: Failed to getTypename. Returning empty string!" << std::endl;
			return "";
		}

		std::string result;
		for (size_t i = 1; i < vec.size(); i++)
		{
			result += vec[i];
		}
		return result;
	}

#define TRISTEON_TYPENAME(T) Tristeon::getTypename<T>()

#else
#include <cxxabi.h>

	/**
	 * Wrapper around typeid(T).name(), to enforce a consistent return style across all (supported) compilers.
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
#define TRISTEON_TYPENAME(T) Tristeon::getTypename<T>()
#endif
}