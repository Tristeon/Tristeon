#include <Actors/Behaviours/TestBehaviour.h>
#include <Actors/Actor.h>

#include <Input/Gamepad.h>
#include <Input/Keyboard.h>

#include <Rendering/GameView.h>

#include <Physics/PhysicsBody.h>

#include "Physics/PhysicsWorld.h"

namespace Tristeon
{
	REGISTER_TYPE_CPP(TestBehaviour)

	json TestBehaviour::serialize()
	{
		json j;
		j["typeID"] = TRISTEON_TYPENAME(TestBehaviour);
		return j;
	}

	void TestBehaviour::deserialize(json j)
	{
		//Empty
	}

	void TestBehaviour::start()
	{
		body = owner()->behaviour<PhysicsBody>();
	}

	void TestBehaviour::update()
	{
		if (Keyboard::pressed(Keyboard::Space) && PhysicsWorld::raycast(body->position(), Vector2::down(), 256))
			body->velocity({body->velocity().x, 1500});

		float const horizontal = Keyboard::held(Keyboard::D) - Keyboard::held(Keyboard::A);
		body->applyForce(Vector2(horizontal, 0) *GameView::deltaTime() * 100);
	}
}
