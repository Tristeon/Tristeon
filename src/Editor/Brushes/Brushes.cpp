#ifdef TRISTEON_EDITOR
#include "Brushes.h"
#include "Stamp.h"

namespace TristeonEditor
{
	Tristeon::Unique<AbstractBrush> Brushes::_current = std::make_unique<Stamp>();
}
#endif