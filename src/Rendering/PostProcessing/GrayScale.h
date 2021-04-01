#pragma once
#include "PostProcessingEffect.h"
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	class GrayScale : public PostProcessingEffect
	{
	public:
		[[nodiscard]] json serialize() override;
		Shader* shader() override;
	};

	REGISTER_TYPE(GrayScale);
}