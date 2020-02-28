#include "SpriteBehaviour.h"
#include <Rendering/Texture.h>

namespace Tristeon
{
	void SpriteBehaviour::setTexture(std::string const& path, bool setSize)
	{
		texturePath = path;
		texture = new Texture(path);

		if (setSize)
		{
			width = texture->width();
			height = texture->height();
		}
	}
}
