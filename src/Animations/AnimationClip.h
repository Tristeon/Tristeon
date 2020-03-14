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
		struct Spacing
		{
			uint left = 0;
			uint right = 0;
			uint top = 0;
			uint bottom = 0;

			uint horizontalFrame = 0;
			uint verticalFrame = 0;
		};
		
		uint rows = 0;
		uint cols = 0;

		uint startIndex = 0;
		uint endIndex = 0;

		Spacing spacing;
		
		bool loops = false;
		
		std::string texturePath = "";

		AnimationClip() = default;
		AnimationClip(std::string const& texturePath, uint const& cols, uint const& rows, bool const& loops, int const& startIndex = -1, int const& endIndex = -1, Spacing const& spacing = { 0, 0, 0, 0, 0, 0 });

		json serialize() override;
		void deserialize(json j) override;
	};
}