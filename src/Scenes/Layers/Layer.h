#pragma once
#include <Serialization/Serializable.h>

namespace Tristeon
{
	class Renderer;
	class Scene;

	class Layer : public Serializable
	{
	public:
		virtual void render(Renderer* renderer, Scene* scene) = 0;
		virtual void update();

		json serialize() override = 0;
		void deserialize(json j) override = 0;
	};
}
