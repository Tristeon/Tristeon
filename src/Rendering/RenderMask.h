#pragma once
#include <Serialization/MetaWrappers/SerializedEnum.h>

namespace Tristeon
{
	enum class RenderMask : uint8_t
	{
		Default = 1,
		Mask1 = 1 << 1,
		Mask2 = 1 << 2,
		Mask3 = 1 << 3,
		Mask4 = 1 << 4,
		Mask5 = 1 << 5,
		Mask6 = 1 << 6,
		All = 127
	};

	ENUM_EDITOR(RenderMask);
}