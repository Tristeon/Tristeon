#pragma once
#include "Rendering/Graphic.h"
#include <Rendering/Texture.h>

#include <Utils/Colour.h>
#include <Serialization/TypeRegister.h>
#include <Serialization/MetaWrappers/TextureField.h>

namespace Tristeon
{
	class ActorLayer;

	enum class TextureType
	{
		Albedo,
		Normal,
		LightMask
	};
	
	/**
	 * Sprite is a 2D quad with a texture. Its size can be adjusted through Sprite::width and Sprite::height.
	 */
	class Sprite : public Graphic
	{
		friend ActorLayer;
	public:		
		/**
		 * Creates a white sprite of size 64x64.
		 */
		Sprite() = default;
		virtual ~Sprite() = default;

		DELETE_COPY(Sprite);
		DEFAULT_MOVE(Sprite);

		json serialize() override;
		void deserialize(json j) override;

		VectorU size { 64, 64 };

		/**
		 * Flip the sprite vertically, or horizontally
		 */
		VectorB flip;

		/**
		 * The colour of the sprite, this acts as a multiplier on top of the existing texture.
		 */
		Colour colour = Colour::white();

		/**
		 * Sets the texture of the sprite to the texture at the given path.
		 *
		 * \param pPath The filepath of the new texture.
		 * \param pSetSize If true, the sprite will take on the size of the texture.
		 * \param pType The type of texture
		 */
		virtual void setTexture(const String& pPath, const bool& pSetSize, const TextureType& pType = TextureType::Albedo);

		/**
		 * Returns the current texture of the sprite. May be nullptr.
		 */
		virtual Texture* texture(const TextureType& pType = TextureType::Albedo);
		
		/**
		 * Returns the Sprite's Bounds.
		 * This is a square, defined using the position, scale, width, and height of the sprite.
		 */
		Bounds bounds() override;

	protected:
		void render() override;
		Shader* getShader() override;

		TextureField _albedo;
		TextureField _normal;
		TextureField _lightMask;
		
		float _normalMapStrength = 1.0f;
	};

	REGISTER_TYPE(Sprite);
}