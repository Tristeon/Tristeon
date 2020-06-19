#pragma once
#ifdef TRISTEON_EDITOR
#include <QString>

#include "Utils/Colour.h"

namespace TristeonEditor
{
	class Palette
	{
	public:
		constexpr static Tristeon::Colour background = Tristeon::Colour(44 / 255.0f, 62 / 255.0f, 80 / 255.0f);
		constexpr static Tristeon::Colour alternateBackground = Tristeon::Colour(52 / 255.0f, 73 / 255.0f, 94 / 255.0f);

		constexpr static Tristeon::Colour text = Tristeon::Colour(236 / 255.0f, 240 / 255.0f, 241 / 255.0f);

		constexpr static Tristeon::Colour add = Tristeon::Colour(46 / 255.0f, 204 / 255.0f, 113 / 255.0f);
		constexpr static Tristeon::Colour remove = Tristeon::Colour(192 / 255.0f, 57 / 255.0f, 43 / 255.0f);

		static QString getRGBString(std::string const& colorType, Tristeon::Colour const& colour)
		{
			return QString("%1:rgb(%2,%3,%4);")
				.arg(colorType.data())
				.arg(colour.r * 255)
				.arg(colour.g * 255)
				.arg(colour.b * 255);
		};
	};
}
#endif