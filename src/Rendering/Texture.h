#pragma once
#include <string>

#include "TObject.h"
#include "Math/Vector2Int.h"

namespace Tristeon
{
	/**
	 * Textures can be attached to Sprites or TileSets.
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
		explicit Texture(std::string const& path);

		virtual ~Texture();

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
		bool loaded() const;

		static const std::string defaultPath;
	private:
		int w = 0;
		int h = 0;
		int c = 0;
		
		bool succeeded = false;
		unsigned int texture = 0;
	};
}
