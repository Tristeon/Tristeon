#pragma once
#include "Layer.h"
#include <vector>
#include <Actors/Actor.h>
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	class SceneManager;
	
	class ActorLayer : public Layer
	{
		REGISTER_TYPE_H(ActorLayer)
		
		friend SceneManager;
	public:
		json serialize() override;
		void deserialize(json j) override;

		void render(Renderer* renderer, Scene* scene) override;
		void update() override;
	private:
		Vector<Unique<Actor>> actors;
	};
}
