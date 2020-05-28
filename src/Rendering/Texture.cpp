#include "Texture.h"
#include <QImage>

namespace Tristeon
{
	const std::string Texture::defaultPath = "Internal/Textures/white.jpg";

	Texture::Texture(QString const& path)
	{
		QImage image;
		bool const loaded = image.load(path);

		if (!loaded)
			image.load(QString::fromStdString(defaultPath));
		else
			succeeded = true;

		texture = new QOpenGLTexture(image.mirrored());
		texture->setWrapMode(QOpenGLTexture::Repeat);
		texture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
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
		return { texture->width(), texture->height() };
	}
}
