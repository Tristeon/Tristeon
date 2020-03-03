#include "AnimationClip.h"

namespace Tristeon
{
	AnimationClip::AnimationClip(std::string const& texturePath, int const& cols, int const& rows, bool const& loops)
		: AnimationClip(texturePath, cols, rows, 0, cols* rows, loops)
	{
		//Empty
	}

	AnimationClip::AnimationClip(std::string const& texturePath, int const& cols, int const& rows,
		int const& startIndex, int const& endIndex, bool const& loops) : texturePath(texturePath), cols(cols), rows(rows), startIndex(startIndex), endIndex(endIndex), loops(loops)
	{
		//Empty
	}
}
