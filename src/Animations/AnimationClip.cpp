#include "AnimationClip.h"
#include "Serialization/JsonSerializer.h"

namespace Tristeon
{
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
		rows = j.value("rows", 1);
		cols = j.value("cols", 1);
		startIndex = j.value("startIndex", 0);
		endIndex = j.value("endIndex", rows * cols);
		loops = j.value("loops", true);
		texturePath = j.value("texturePath", "");
		playbackRate = j.value("playbackRate", 1.0f);

		const json s = j.value("spacing", json());
		spacing.left = s.value("left", 0);
		spacing.right = s.value("right", 0);
		spacing.top = s.value("top", 0);
		spacing.bottom = s.value("bottom", 0);
		spacing.horizontalFrame = s.value("horizontalFrame", 0);
		spacing.verticalFrame = s.value("verticalFrame", 0);
	}
}
