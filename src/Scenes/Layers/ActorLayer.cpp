#include "ActorLayer.h"

#include <QOpenGLFunctions>
#include <Scenes/Scene.h>
#include <Rendering/Renderer.h>

#include <Actors/Behaviours/BehaviourCollector.h>
#include <Input/Keyboard.h>

namespace Tristeon
{
	REGISTER_TYPE_CPP(ActorLayer)
	
	json ActorLayer::serialize()
	{
		json j;
		j["typeID"] = TRISTEON_TYPENAME(ActorLayer);

		json serializedActors = json::array_t();
		for (size_t i = 0; i < actors.size(); i++)
			serializedActors.push_back(actors[i]->serialize());
		j["actors"] = serializedActors;
		
		return j;
	}

	void ActorLayer::deserialize(json j)
	{
		actors.clear();
		
		for (auto serializedActor : j["actors"])
		{
			//TODO: Potentially detect existing actors and simply re-deserialize as opposed to recreating them
			Unique<Serializable> serializable = TypeRegister::createInstance(serializedActor["typeID"]);
			serializable->deserialize(serializedActor);
			actors.push_back(Unique<Actor>((Actor*)serializable.release()));
		}
	}

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
