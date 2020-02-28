#pragma once
#include "Behaviours/SpriteBehaviour.h"
#include "Math/Vector2Int.h"

namespace Tristeon
{
	class ActorLayer;
	class SceneManager;
	
	class Actor
	{
		friend ActorLayer;
		friend SceneManager;
	public:
		Vector2 position = { 0, 0 };
		Vector2 scale = { 0, 0 };
		float rotation = 0;

		SpriteBehaviour* sprite() const { return _sprite.get(); }
	private:
		std::unique_ptr<SpriteBehaviour> _sprite = nullptr;
	};
}
