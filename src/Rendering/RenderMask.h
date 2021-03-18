#pragma once

namespace Tristeon
{
	enum class RenderMask : uint8_t
	{
		None = 0,
		Default = 1,
		Mask1 = 1 << 1,
		Mask2 = 1 << 2,
		Mask3 = 1 << 3,
		Mask4 = 1 << 4,
		Mask5 = 1 << 5,
		Mask6 = 1 << 6,
		Mask7 = 1 << 7,
		All = 255
	};
}