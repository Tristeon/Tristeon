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

		std::string getPath() const { return imagePath.toStdString(); }
		
		void bind();

		int width()
		{
			if (!loaded) 
				load();
			
			return texture->width();
		}
		
		int height()
		{
			if (!loaded)
				load();
			
			return texture->height();
		}
	private:
		void load();
		
		bool loaded = false;
		QString imagePath = "";
		QOpenGLTexture* texture = nullptr;
	};
}
