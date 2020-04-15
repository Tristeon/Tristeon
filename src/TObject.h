#pragma once

namespace Tristeon
{
	/**
	 * TObject is a plain polymorphic class used as a base class for most classes in Tristeon.
	 * It's often used to be able to deduce types from void pointers by initially casting to TObject and then dynamically casting to other types.
	 */
	class TObject
	{
	public:
		virtual ~TObject() = default;
	};
}