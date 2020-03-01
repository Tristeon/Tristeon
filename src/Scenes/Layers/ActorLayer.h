#pragma once
#include "Layer.h"
#include <memory>
#include <vector>

#include "Actors/Actor.h"

namespace Tristeon
{
	class SceneManager;
	
	class ActorLayer : public Layer
	{
		friend SceneManager;
	public:
		void render(Renderer* renderer, Scene* scene) override;
		void update() override;
	private:
		std::vector<std::unique_ptr<Actor>> actors;
	};
}
