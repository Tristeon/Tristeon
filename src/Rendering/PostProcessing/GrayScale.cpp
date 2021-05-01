#include "GrayScale.h"

namespace Tristeon
{
	json GrayScale::serialize()
	{
		json j = PostProcessingEffect::serialize();
		j["typeID"] = Type<GrayScale>::fullName();
		return j;
	}

	Shader* GrayScale::shader()
	{
		static Shader s{ "Engine://Shaders/FullscreenTriangle.vert", "Engine://Shaders/PostProcessing/GrayScale.frag" };
		return &s;
	}
}