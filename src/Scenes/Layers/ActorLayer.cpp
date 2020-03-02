#include "ActorLayer.h"

#include <QOpenGLFunctions>
#include <Scenes/Scene.h>
#include <Rendering/Renderer.h>

#include "Actors/Behaviours/BehaviourCollector.h"
#include "Input/Keyboard.h"

namespace Tristeon
{
	void ActorLayer::render(Renderer* renderer, Scene* scene)
	{
		if (!renderer->getSpriteShader()->isReady())
			return;

		if (Keyboard::pressed(Key_R))
			renderer->getSpriteShader()->reload();

		QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
		auto program = renderer->getSpriteShader()->getShaderProgram();
		program->bind();
		f->glActiveTexture(0);

		//Camera
		program->setUniformValue("camera.posX", scene->getCamera()->position.x);
		program->setUniformValue("camera.posY", scene->getCamera()->position.y);
		program->setUniformValue("camera.pixelsX", (int)scene->getCamera()->size.x);
		program->setUniformValue("camera.pixelsY", (int)scene->getCamera()->size.y);
		program->setUniformValue("camera.zoom", scene->getCamera()->zoom);

		for (Sprite* sprite : BehaviourCollector<Sprite>::all())
		{
			Actor* actor = sprite->owner();
			if (sprite == nullptr)
				continue;
			
			sprite->texture->bind();

			//Sprite info
			program->setUniformValue("renderMode", sprite->renderMode);
			program->setUniformValue("sprite.width", sprite->width);
			program->setUniformValue("sprite.height", sprite->height);
			program->setUniformValue("actor.position", actor->position.x, actor->position.y);
			program->setUniformValue("actor.scale", actor->scale.x, actor->scale.y);
			program->setUniformValue("actor.rotation", actor->rotation);

			//Animation
			f->glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}

	void ActorLayer::update()
	{
		for (auto& actor : actors)
			for (auto& behaviour : actor->behaviours)
				behaviour->update();
	}
}