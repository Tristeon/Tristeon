#pragma once
#include "CompositeLight.h"
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	class Texture;

	class SpriteLight : public CompositeLight
	{
		friend class Renderer;
	public:
		[[nodiscard]] json serialize() override;
		void deserialize(json j) override;

	protected:
		void render() override;
		Shader* shader() override;

		unsigned int _width = 0;
		unsigned int _height = 0;

		bool _flipX = false;
		bool _flipY = false;

		Texture* _texture = nullptr;
		String _texturePath;
	};
	
	REGISTER_TYPE(SpriteLight);
}