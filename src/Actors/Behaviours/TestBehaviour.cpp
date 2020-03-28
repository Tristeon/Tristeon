#include <Actors/Behaviours/TestBehaviour.h>
#include <Actors/Actor.h>

#include <Input/Gamepad.h>
#include <Input/Keyboard.h>

#include <Rendering/GameView.h>

#include <Physics/PhysicsBody.h>

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
		if (Gamepad::pressed(Gamepad::A))
			body->velocity({body->velocity().x, 1500});
		
		body->applyForce(Gamepad::axisLeft() * GameView::deltaTime() * 100);
	}
}
