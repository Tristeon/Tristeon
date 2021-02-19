#include "AnimationClip.h"
#include "Serialization/JsonSerializer.h"

namespace Tristeon
{
	json AnimationClip::serialize()
	{
		json j = Serializable::serialize();
		j["typeID"] = Type<AnimationClip>::fullName();
		j["rows"] = rows;
		j["cols"] = cols;
		j["startIndex"] = startIndex;
		j["endIndex"] = endIndex;
		j["loops"] = loops;
		j["texturePath"] = albedoPath;
		j["normalPath"] = normalPath;
		j["normalMapStrength"] = normalMapStrength;
		j["lightMaskPath"] = lightMaskPath;
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
		Serializable::deserialize(j);
		
		rows = j.value("rows", 1u);
		cols = j.value("cols", 1u);
		startIndex = j.value("startIndex", 0);
		endIndex = j.value("endIndex", rows * cols);
		loops = j.value("loops", true);
		albedoPath = j.value("texturePath", "");
		normalPath = j.value("normalPath", "");
		normalMapStrength = j.value("normalMapStrength", 1.0f);
		lightMaskPath = j.value("lightMaskPath", "");
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
