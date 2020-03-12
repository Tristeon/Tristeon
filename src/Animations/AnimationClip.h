#pragma once
#include "Rendering/Texture.h"

#include <Serialization/Serializable.h>
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	class AnimationClip : public Serializable
	{
		REGISTER_TYPE_H(AnimationClip)
	public:
		unsigned int rows = 0;
		unsigned int cols = 0;

		unsigned int startIndex = 0;
		unsigned int endIndex = 0;

		unsigned int horizontalFrameSpacing = 0;
		unsigned int verticalFrameSpacing = 0;

		bool loops = false;
		
		std::string texturePath = "";

		AnimationClip() = default;
		AnimationClip(std::string const& texturePath, unsigned int const& cols, unsigned int const& rows, bool const& loops, int const& startIndex = -1, int const& endIndex = -1, uint const& horizontalFrameSpacing = 0, uint const& verticalFrameSpacing = 0);

		json serialize() override;
		void deserialize(json j) override;
	};
}