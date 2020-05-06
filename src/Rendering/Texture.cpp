#include "Texture.h"
#include <QImage>

namespace Tristeon
{
	REGISTER_TYPE_CPP(Texture);
	
	const std::string Texture::defaultPath = "Internal/Textures/white.jpg";

	json Texture::serialize()
	{
		json j;
		j["typeID"] = TRISTEON_TYPENAME(Texture);
		j["filePath"] = imagePath.toStdString();
		return j;
	}

	void Texture::deserialize(json j)
	{
		imagePath = QString::fromStdString(j["filePath"].get<std::string>());
		if (texture != nullptr)
		{
			delete texture;
			texture = nullptr;
		}
		load();
	}

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
		return { texture->width(), texture->height() };
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