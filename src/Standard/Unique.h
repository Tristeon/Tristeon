#pragma once
#include <memory>

namespace Tristeon
{
	template<typename T, typename D = std::default_delete<T>>
	using Unique = std::unique_ptr<T, D>;
}