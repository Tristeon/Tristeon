#pragma once
#ifdef TRISTEON_EDITOR
#include "AbstractBrush.h"

namespace TristeonEditor
{
	class Brushes
	{
	public:
		static AbstractBrush* current() { return _current.get(); }
	private:
		static Tristeon::Unique<AbstractBrush> _current;
	};
}
#endif