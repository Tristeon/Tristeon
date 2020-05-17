#include "Sprite.h"

#include <Actors/Actor.h>
#include <Rendering/Texture.h>

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <Resources.h>

#include "Math/Math.h"

namespace Tristeon
{
	REGISTER_ACTOR_CPP(Sprite)
	REGISTER_TYPE_CPP(Sprite)
	
	Sprite::Sprite()
	{
		texture = Resources::load<Texture>("Internal/Textures/white.jpg");
	}

	json Sprite::serialize()
	{
		json j = Actor::serialize();
		j["typeID"] = TRISTEON_TYPENAME(Sprite);
		j["width"] = width;
		j["height"] = height;
		j["colour"] = colour;
		j["texturePath"] = texture->getPath();
		return j;
	}

	void Sprite::deserialize(json j)
	{
		Actor::deserialize(j);
		
		width = j["width"];
		height = j["height"];

		colour = j["colour"];

		std::string texturePath = j["texturePath"];
		if (texturePath != texture->getPath()) //Update if new path
			texture = Resources::load<Texture>(texturePath);
	}

	void Sprite::setTexture(std::string const& path, bool const& setSize)
	{
		texture = Resources::load<Texture>(path);

		if (setSize)
		{
			width = texture->width();
			height = texture->height();
		}
	}

	void Sprite::render(QOpenGLShaderProgram* program)
	{
		QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
		f->glActiveTexture(GL_TEXTURE0);
		texture->bind();

		//Sprite info
		program->setUniformValue("sprite.width", width);
		program->setUniformValue("sprite.height", height);
		program->setUniformValue("sprite.colour", colour.r, colour.g, colour.b, colour.a);
		
		program->setUniformValue("actor.position", position.x, position.y);
		program->setUniformValue("actor.scale", scale.x, scale.y);
		program->setUniformValue("actor.rotation", -rotation);

		f->glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	Shader* Sprite::getShader()
	{
		static Shader shader = Shader("Internal/Shaders/Sprite.vert", "Internal/Shaders/Sprite.frag");
		return &shader;
	}

	bool Sprite::withinBounds(Vector2 const& worldPos)
	{
		Vector2 const halfSize = { width / 2.0f * scale.x, height / 2.0f * scale.y };
		return AABB{ position - halfSize, position + halfSize }.contains(worldPos);
	}

	Graphic::AABB Sprite::getAABB()
	{
		float const halfW = width / 2.0f * scale.x;
		float const halfH = height / 2.0f * scale.y;

		Vector2 const bottomLeft = Math::orbit(position, Vector2(-halfW, -halfH), rotation);
		Vector2 const bottomRight = Math::orbit(position, Vector2(halfW, -halfH), rotation);
		Vector2 const topLeft = Math::orbit(position, Vector2(-halfW, halfH), rotation);
		Vector2 const topRight = Math::orbit(position, Vector2(halfW, halfH), rotation);

		float const minX = std::min({ bottomLeft.x, bottomRight.x, topLeft.x, topRight.x });
		float const minY = std::min({ bottomLeft.y, bottomRight.y, topLeft.y, topRight.y });

		float const maxX = std::max({ bottomLeft.x, bottomRight.x, topLeft.x, topRight.x });
		float const maxY = std::max({ bottomLeft.y, bottomRight.y, topLeft.y, topRight.y });

		return AABB{ { minX, minY }, { maxX, maxY } };
	}
}
