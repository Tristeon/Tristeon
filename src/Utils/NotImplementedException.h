#pragma once
#include <stdexcept>

namespace Tristeon
{
	/**
	 * NotImplementedException is a logic_error used for generated functions that ought to be implemented. 
	 */
	class NotImplementedException : std::logic_error
	{
	public:
		explicit NotImplementedException() : logic_error("Function hasn't been implemented yet")
		{
			//Empty
		}
	};
}
