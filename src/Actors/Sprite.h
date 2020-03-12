#pragma once
#include "Actor.h"
#include <Rendering/SpriteRenderMode.h>
#include <Rendering/Texture.h>

#include <Serialization/TypeRegister.h>

class QOpenGLShaderProgram;

namespace Tristeon
{
	class ActorLayer;
	
	class Sprite : public Actor
	{
		REGISTER_TYPE_H(Sprite)
		
		friend ActorLayer;
	public:
		Sprite();
		virtual ~Sprite();

		json serialize() override;
		void deserialize(json j) override;

		int width = 64;
		int height = 64;

		int spacingLeft = 0;
		int spacingRight = 0;
		int spacingTop = 0;
		int spacingBottom = 0;

		virtual SpriteRenderMode getRenderMode();
		void setTexture(std::string const& path, bool setSize);
	protected:
		virtual void render(QOpenGLShaderProgram* program);

		std::unique_ptr<Texture> texture = nullptr;
	};
}
