#pragma once
#include "Actor.h"

#include <Rendering/Texture.h>
#include <Serialization/TypeRegister.h>
#include <Rendering/Graphic.h>

#include <Rendering/Shader.h>

class QOpenGLShaderProgram;

namespace Tristeon
{
	class ActorLayer;
	
	class Sprite : public Graphic
	{
		REGISTER_TYPE_H(Sprite)
		
		friend ActorLayer;
	public:
		Sprite();
		virtual ~Sprite() = default;

		json serialize() override;
		void deserialize(json j) override;

		int width = 64;
		int height = 64;
		
		void setTexture(std::string const& path, bool setSize);
	protected:
		virtual void render(QOpenGLShaderProgram* program) override;
		virtual Shader* getShader() override;

		std::unique_ptr<Texture> texture = nullptr;
	};
}
