#pragma once
#include <QOpenGLTexture>
#include <string>
#include <QString>

namespace Tristeon
{
	class Texture
	{
	public:
		explicit Texture() : Texture("Internal/Textures/white.jpg") { }
		explicit Texture(std::string const& path) : Texture(QString(path.c_str())) { }
		explicit Texture(char* const& path) : Texture(QString(path)) { }
		explicit Texture(QString const& path);

		virtual ~Texture();
		
		bool isLoaded() const { return loaded; }
		QOpenGLTexture* getQTexture() const { return texture; }

		void bind();
	private:
		void load();
		
		bool loaded = false;
		QString imagePath = "";
		QOpenGLTexture* texture = nullptr;
	};
}
