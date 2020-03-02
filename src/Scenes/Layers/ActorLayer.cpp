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
			if (sprite == nullptr)
				continue;
			sprite->render(program);
		}
	}

	void ActorLayer::update()
	{
		for (auto& actor : actors)
			for (auto& behaviour : actor->behaviours)
				behaviour->update();
	}
}