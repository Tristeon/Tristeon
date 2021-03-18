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

		[[nodiscard]] unsigned int width() const { return _width; }
		void setWidth(const unsigned int& pValue) { _width = pValue; }

		[[nodiscard]] unsigned int height() const { return _height; }
		void setHeight(const unsigned int& pValue) { _height = pValue; }

		[[nodiscard]] bool flipX() const { return _flipX; }
		void setFlipX(const bool& pValue) { _flipX = pValue; }

		[[nodiscard]] bool flipY() const { return _flipY; }
		void setFlipY(const bool& pValue) { _flipY = pValue; }

		[[nodiscard]] String texturePath() const { return _texturePath; }
		void setTexture(const String& pPath, const bool& pSetSize = false);
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