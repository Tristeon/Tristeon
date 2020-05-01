// ReSharper disable All
#include <Actors/Behaviours/TestBehaviour.h>
#include <Actors/Actor.h>

#include <Input/Gamepad.h>
#include <Input/Keyboard.h>

#include <Rendering/GameView.h>

#include <Physics/PhysicsBody.h>

#include "Physics/Collider.h"
#include "Physics/PhysicsWorld.h"

namespace Tristeon
{
	REGISTER_BEHAVIOUR_CPP(TestBehaviour)
	REGISTER_TYPE_CPP(TestBehaviour)

	void TestBehaviour::start()
	{
		body = owner()->behaviour<PhysicsBody>();
	}

	void TestBehaviour::update()
	{
		bool grounded = PhysicsWorld::raycast(owner()->position, Vector2::down(), groundedDistance);
		if (Keyboard::pressed(Keyboard::Space) && grounded)
			body->velocity({ body->velocity().x, jumpVelocity });

		float const horizontal = Keyboard::held(Keyboard::D) - Keyboard::held(Keyboard::A);
		body->applyForce(Vector2(horizontal, 0) * GameView::deltaTime() * runSpeed);
	}
	
	json TestBehaviour::serialize()
	{
		json j;
		j["typeID"] = TRISTEON_TYPENAME(TestBehaviour);
		j["jumpVelocity"] = jumpVelocity;
		j["groundedDistance"] = groundedDistance;
		j["runSpeed"] = runSpeed;
		return j;
	}

	void TestBehaviour::deserialize(json j)
	{
		jumpVelocity = j["jumpVelocity"];
		groundedDistance = j["groundedDistance"];
		runSpeed = j["runSpeed"];
	}

	void TestBehaviour::contactBegin(Collider* other)
	{
		std::cout << "Penguin collided with: " + other->owner()->name << std::endl;
	}

	void TestBehaviour::contactEnd(Collider* other)
	{
		std::cout << "Penguin stopped colliding with: " + other->owner()->name << std::endl;
	}

	void TestBehaviour::tileContactBegin(TileContact const& contact)
	{
		std::cout << "Penguin collided with tile " << contact.tileIndex.toString() << std::endl;
	}

	void TestBehaviour::tileContactEnd(TileContact const& contact)
	{
		std::cout << "Penguin stopped colliding with tile " << contact.tileIndex.toString() << std::endl;
	}
}
