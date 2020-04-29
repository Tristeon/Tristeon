#include "Texture.h"
#include <QImage>

namespace Tristeon
{
	const std::string Texture::defaultPath = "Internal/Textures/white.jpg";
	
	Texture::Texture(QString const& path) : imagePath(path)
	{
		load();
	}

	Texture::~Texture()
	{
		delete texture;
	}

	void Texture::bind() const
	{
		texture->bind();
	}

	int Texture::width() const
	{
		return texture->width();
	}

	int Texture::height() const
	{
		return texture->height();
	}

	Vector2Int Texture::size() const
	{
		return Vector2Int(texture->width(), texture->height());
	}

	void Texture::load()
	{
		QImage image;
		bool const loaded = image.load(imagePath);

		if (!loaded)
			image.load(QString("Internal/Textures/white.jpg"));
		else
			succeeded = true;
		
		texture = new QOpenGLTexture(image.mirrored());
		texture->setWrapMode(QOpenGLTexture::Repeat);
		texture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
	}
}
