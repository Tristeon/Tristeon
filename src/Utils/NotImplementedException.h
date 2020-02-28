#pragma once
#include <stdexcept>

namespace Tristeon
{
	class NotImplementedException : std::logic_error
	{
	public:
		explicit NotImplementedException() : logic_error("Function hasn't been implemented yet")
		{
			//Empty
		}
	};
}
