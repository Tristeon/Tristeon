#pragma once
#include "Rendering/Texture.h"

namespace Tristeon
{
	enum SpriteRenderMode
	{
		Standard,
		Animated,
	};

	class ActorLayer;
	
	class SpriteBehaviour
	{
		friend ActorLayer;
	public:
		int width = 64;
		int height = 64;
		SpriteRenderMode renderMode = Standard;
		
		void setTexture(std::string const& path, bool setSize);
	private:
		std::string texturePath = "";
		Texture* texture = nullptr;
	};
}
