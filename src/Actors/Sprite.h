#pragma once
#include "Actor.h"

#include <Rendering/Graphic.h>
#include <Rendering/Texture.h>
#include <Serialization/TypeRegister.h>
#include <Registers/ActorRegister.h>

#include <Rendering/Shader.h>

#include "Utils/Colour.h"

class QOpenGLShaderProgram;

namespace Tristeon
{
	class ActorLayer;

	/**
	 * Sprite is a 2D quad with a texture. Its size can be adjusted through Sprite::width and Sprite::height.
	 */
	class Sprite : public Graphic
	{
		REGISTER_ACTOR_H(Sprite)
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
		 * If true, the sprite is flipped horizontally
		 */
		bool flipX = false;

		/**
		 * If true, the sprite is flipped vertically
		 */
		bool flipY = false;

		/**
		 * The colour of the sprite, this acts as a multiplier on top of the existing texture.
		 */
		Colour colour = Colour::white();

		/**
		 * Sets the texture of the sprite to the texture at the given path.
		 *
		 * \param path The filepath of the new texture.
		 * \param setSize If true, the sprite will take on the size of the texture.
		 */
		virtual void setTexture(std::string const& path, bool const& setSize);

		/**
		 * Returns the current texture of the sprite. May be nullptr.
		 */
		virtual Texture* getTexture();
		
		/**
		 * Returns true if the world position is within the bounds of the Sprite.
		 */
		bool withinBounds(Vector2 const& worldPos) override;

		/**
		 * Returns the Sprite's AABB.
		 * This is a square, defined using the position, rotation, scale, width and height of the sprite.
		 * This function handles rotation by calculating the smallest AABB fit around the rotated Sprite.
		 */
		AABB getAABB() override;

	protected:
		virtual void render(QOpenGLShaderProgram* program) override;
		virtual Shader* getShader() override;
	protected:
		/**
		 * The 2D texture of the sprite.
		 */
		Texture* texture = nullptr;
	};
}
