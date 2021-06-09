#include "AnimationClip.h"
#include "Serialization/JsonSerializer.h"

namespace Tristeon
{
	json AnimationClip::serialize()
	{
		auto j = Serializable::serialize();
		j["typeID"] = Type<AnimationClip>::fullName();
		j["rows"] = rows;
		j["cols"] = cols;
		j["startIndex"] = startIndex;
		j["endIndex"] = endIndex;
		j["loops"] = loops;
		j["albedo"] = albedo;
		j["normal"] = normal;
		j["lightMask"] = lightMask;
		j["normalMapStrength"] = normalMapStrength;
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
		
		albedo = j.value("albedo", TextureField(0));
		normal = j.value("normal", TextureField(0));
		lightMask = j.value("lightMask", TextureField(0));
		normalMapStrength = j.value("normalMapStrength", 1.0f);

		playbackRate = j.value("playbackRate", 1.0f);

		const auto s = j.value("spacing", json());
		spacing.left = s.value("left", 0);
		spacing.right = s.value("right", 0);
		spacing.top = s.value("top", 0);
		spacing.bottom = s.value("bottom", 0);
		spacing.horizontalFrame = s.value("horizontalFrame", 0);
		spacing.verticalFrame = s.value("verticalFrame", 0);
	}
}
