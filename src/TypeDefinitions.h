#pragma once
#include <memory>
#include <string>
#include <vector>

namespace Tristeon
{
	template<typename T>
	using Vector = std::vector<T>;

	template<typename T>
	using Unique = std::unique_ptr<T>;

	using String = std::string;
}
