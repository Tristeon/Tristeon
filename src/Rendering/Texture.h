#pragma once
#include "TObject.h"
#include <Utils/ClassDefaults.h>
#include "Math/Vector.h"
#include <Standard/String.h>

namespace Tristeon
{
	/**
	 * Textures can be attached to Sprites or Tilesets.
	 *
	 * Textures are created with a filepath in the form of a string.
	 * This path is relative to the bin/ folder in the Tristeon project.
	 */
	class Texture : public TObject
	{
	public:
		/**
		 * Creates a default (white) texture.
		 */
		explicit Texture() : Texture(defaultPath) { }
		/**
		 * Creates a texture with an image at the given filepath.
		 */
		explicit Texture(String const& path);
		virtual ~Texture();

		DELETE_COPY(Texture);
		DEFAULT_MOVE(Texture);
		
		/**
		 * Binds the texture to prepare it for usage within rendering.
		 */
		void bind() const;

		/**
		 * Returns the width of the texture in pixels.
		 */
		[[nodiscard]] unsigned int width() const;

		/**
		 * Returns the height of the texture in pixels.
		 */
		[[nodiscard]] unsigned int height() const;

		/**
		 * Returns the width and height of the texture in pixels.
		 */
		[[nodiscard]] VectorI size() const;

		/**
		 * Returns true if the texture was successfully loaded from the given filepath.
		 */
		[[nodiscard]] bool loaded() const;

		static const String defaultPath;
	private:
		unsigned int _width = 0;
		unsigned int _height = 0;
		unsigned int _channels = 0;
		
		bool _valid = false;
		unsigned int _texture = 0;
	};
}