#pragma once
//Tristeon type definitions for ease of use throughout the engine.

#include <memory>
#include <string>
#include <Utils/List.h>

namespace Tristeon
{
	template<typename T, typename D = std::default_delete<T>>
	using Unique = std::unique_ptr<T, D>;

	using String = std::string;
}
