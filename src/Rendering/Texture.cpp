#include "Texture.h"
#include <QImage>

namespace Tristeon
{
	Texture::Texture(QString const& path) : imagePath(path)
	{

	}

	Texture::~Texture()
	{
		delete texture;
	}

	void Texture::bind()
	{
		if (!loaded)
			load();
		
		texture->bind();
	}

	void Texture::load()
	{
		if (loaded)
			delete texture;
		
		QImage image;
		loaded = image.load(imagePath);

		if (!loaded)
			image.load(QString("Internal/Textures/white.jpg"));

		texture = new QOpenGLTexture(image.mirrored());
		texture->setWrapMode(QOpenGLTexture::Repeat);
		texture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
		loaded = true;
	}
}
