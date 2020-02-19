#include "Texture.h"
#include <QImage>
#include <QOpenGLTexture>
#include "GLContext.h"

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
		if (!GLContext::isReady())
			return;
		
		GLContext::makeCurrent();

		QImage image;
		loaded = image.load(imagePath);

		if (!loaded)
			image.load(QString("Internal/Textures/white.jpg"));

		texture = new QOpenGLTexture(image.mirrored());
		texture->setWrapMode(QOpenGLTexture::Repeat);
		loaded = true;
	}
}
