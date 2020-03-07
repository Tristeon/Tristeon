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
		int rows = 0;
		int cols = 0;

		int startIndex = 0;
		int endIndex = 0;

		bool loops = false;
		
		std::string texturePath = "";

		AnimationClip() = default;
		AnimationClip(std::string const& texturePath, int const& cols, int const& rows, bool const& loops);
		AnimationClip(std::string const& texturePath, int const& cols, int const& rows, int const& startIndex, int const& endIndex, bool const& loops);

		json serialize() override;
		void deserialize(json j) override;
	};
}