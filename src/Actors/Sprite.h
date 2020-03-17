#pragma once
#include "Actor.h"

#include <Rendering/Graphic.h>
#include <Rendering/Texture.h>
#include <Serialization/TypeRegister.h>

#include <Rendering/Shader.h>

class QOpenGLShaderProgram;

namespace Tristeon
{
	class ActorLayer;

	/**
	 * Sprite is a 2D quad with a texture. Its size can be adjusted through Sprite::width and Sprite::height.
	 */
	class Sprite : public Graphic
	{
		REGISTER_TYPE_H(Sprite)
		
		friend ActorLayer;
	public:
		/**
		 * Creates a white sprite of size 64x64.
		 */
		Sprite();
		virtual ~Sprite() = default;

		json serialize() override;
		void deserialize(json j) override;

		/**
		 * The width of the sprite in pixels.
		 */
		int width = 64;

		/**
		 * The height of the sprite in pixels.
		 */
		int height = 64;

		/**
		 * Sets the texture of the sprite to the texture at the given path.
		 *
		 * \param path The filepath of the new texture.
		 * \param setSize If true, the sprite will take on the size of the texture.
		 */
		void setTexture(std::string const& path, bool setSize);
	protected:
		virtual void render(QOpenGLShaderProgram* program) override;
		virtual Shader* getShader() override;

		/**
		 * The 2D texture of the sprite.
		 */
		std::unique_ptr<Texture> texture = nullptr;
	};
}
