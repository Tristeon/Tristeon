#pragma once
#include "Behaviour.h"
#include "Rendering/SpriteRenderMode.h"
#include "Rendering/Texture.h"

class QOpenGLShaderProgram;

namespace Tristeon
{
	class ActorLayer;
	
	class Sprite : public Behaviour
	{
		friend ActorLayer;
	public:
		Sprite();
		virtual ~Sprite();
		int width = 64;
		int height = 64;

		virtual SpriteRenderMode getRenderMode();
		void setTexture(std::string const& path, bool setSize);
	protected:
		virtual void render(QOpenGLShaderProgram* program);
		
		std::string texturePath = "";
		std::unique_ptr<Texture> texture = nullptr;
	};
}
