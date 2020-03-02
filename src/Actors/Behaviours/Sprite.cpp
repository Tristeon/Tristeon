#include "Sprite.h"
#include <Rendering/Texture.h>

#include "BehaviourCollector.h"

namespace Tristeon
{
	Sprite::Sprite()
	{
		BehaviourCollector<Sprite>::add(this);
	}

	Sprite::~Sprite()
	{
		BehaviourCollector<Sprite>::remove(this);
	}

	void Sprite::setTexture(std::string const& path, bool setSize)
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
