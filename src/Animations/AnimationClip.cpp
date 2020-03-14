#include "AnimationClip.h"

namespace Tristeon
{
	REGISTER_TYPE_CPP(AnimationClip)

	AnimationClip::AnimationClip(std::string const& texturePath, 
		uint const& cols, 
		uint const& rows,
		bool const& loops, 
		int const& startIndex, 
		int const& endIndex,
		Spacing const& spacing) : rows(rows), cols(cols), spacing(spacing), loops(loops), texturePath(texturePath)
	{
		this->startIndex = startIndex == -1 ? 0 : startIndex;
		this->endIndex = endIndex == -1 ? cols * rows : endIndex;
	}

	json AnimationClip::serialize()
	{
		json j;
		j["typeID"] = TRISTEON_TYPENAME(AnimationClip);
		j["rows"] = rows;
		j["cols"] = cols;
		j["startIndex"] = startIndex;
		j["endIndex"] = endIndex;
		j["loops"] = loops;
		j["texturePath"] = texturePath;

		j["spacing"]["left"] = spacing.left;
		j["spacing"]["right"] = spacing.right;
		j["spacing"]["top"] = spacing.top;
		j["spacing"]["bottom"] = spacing.bottom;
		j["spacing"]["horizontalFrame"] = spacing.horizontalFrame;
		j["spacing"]["verticalFrame"] = spacing.verticalFrame;

		return j;
	}

	void AnimationClip::deserialize(json j)
	{
		rows = j["rows"];
		cols = j["cols"];
		startIndex = j["startIndex"];
		endIndex = j["endIndex"];
		loops = j["loops"];
		texturePath = j["texturePath"].get<std::string>();

		spacing.left = j["spacing"]["left"];
		spacing.right = j["spacing"]["right"];
		spacing.top = j["spacing"]["top"];
		spacing.bottom = j["spacing"]["bottom"];
		spacing.horizontalFrame = j["spacing"]["horizontalFrame"];
		spacing.verticalFrame = j["spacing"]["verticalFrame"];
	}
}
