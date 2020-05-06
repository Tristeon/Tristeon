#pragma once
#include <QOpenGLTexture>
#include <QString>
#include <string>

#include "Math/Vector2Int.h"
#include <Serialization/Serializable.h>
#include <Serialization/TypeRegister.h>
namespace Tristeon
{
	/**
	 * Textures can be attached to Sprites or TileSets.
	 *
	 * Textures are created with a filepath in the form of a string.
	 * This path is relative to the bin/ folder in the Tristeon project.
	 */
	class Texture : public Serializable
	{
		REGISTER_TYPE_H(Texture);
		
	public:
		json serialize() override;
		void deserialize(json j) override;

		/**
		 * Creates a default (white) texture.
		 */
		explicit Texture() : Texture("Internal/Textures/white.jpg") { }
		/**
		 * Creates a texture with an image at the given filepath.
		 */
		explicit Texture(std::string const& path) : Texture(QString(path.c_str())) { }
		/**
		 * Creates a texture with an image at the given filepath.
		 */
		explicit Texture(char* const& path) : Texture(QString(path)) { }
		/**
		 * Creates a texture with an image at the given filepath.
		 */
		explicit Texture(QString const& path);

		virtual ~Texture();

		/**
		 * Returns the QOpenGLTexture, used internally with interactions with Qt
		 */
		QOpenGLTexture* getQTexture() const { return texture; }

		/**
		 * Returns the filepath, set in the constructor
		 */
		std::string getPath() const { return imagePath.toStdString(); }

		/**
		 * Binds the texture to prepare it for usage within rendering.
		 */
		void bind() const;

		/**
		 * Returns the width of the texture in pixels.
		 */
		int width() const;

		/**
		 * Returns the height of the texture in pixels.
		 */
		int height() const;

		/**
		 * Returns the width and height of the texture in pixels.
		 */
		Vector2Int size() const;

		/**
		 * Returns true if the texture was successfully loaded from the given filepath.
		 */
		bool loaded() const { return succeeded; }

		static const std::string defaultPath;
	private:
		void load();

		bool succeeded = false;
		QString imagePath = "";
		QOpenGLTexture* texture = nullptr;
	};
}