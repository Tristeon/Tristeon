#pragma once
//Tristeon type definitions for ease of use throughout the engine.

#include <memory>
#include <string>
#include <Utils/Vector.h>

namespace Tristeon
{
	template<typename T>
	using Unique = std::unique_ptr<T>;

	using String = std::string;
}
