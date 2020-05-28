#include "AnimationClip.h"
#include "Serialization/JsonSerializer.h"

namespace Tristeon
{
	REGISTER_TYPE_CPP(AnimationClip)

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
		j["playbackRate"] = playbackRate;

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
		playbackRate = j.contains("playbackRate") ? j["playbackRate"] : 1;

		spacing.left = j["spacing"]["left"];
		spacing.right = j["spacing"]["right"];
		spacing.top = j["spacing"]["top"];
		spacing.bottom = j["spacing"]["bottom"];
		spacing.horizontalFrame = j["spacing"]["horizontalFrame"];
		spacing.verticalFrame = j["spacing"]["verticalFrame"];
	}
}
