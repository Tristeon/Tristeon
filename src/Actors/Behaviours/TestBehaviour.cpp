#include <Actors/Behaviours/TestBehaviour.h>
#include <Actors/Actor.h>

#include <Input/Gamepad.h>
#include <Input/Keyboard.h>
#include <Physics/PhysicsBody.h>


#include "Actors/Camera.h"
#include "Actors/Sprite.h"
#include "Physics/BoxCollider.h"
#include "Physics/Collider.h"
#include "Physics/PhysicsWorld.h"
#include "Rendering/Gizmos.h"
#include "Utils/Time.h"

namespace Tristeon
{
	void TestBehaviour::start()
	{
		body = actor()->behaviour<PhysicsBody>();
	}

	void TestBehaviour::update()
	{
		const auto grounded = PhysicsWorld::raycast(actor()->position, Vector2::down(), groundedDistance);
		if (Keyboard::pressed(Keyboard::Space) && grounded)
			body->setVelocity({ body->velocity().x, jumpVelocity });

		float const horizontal = Keyboard::held(Keyboard::D) - Keyboard::held(Keyboard::A);
		body->applyForce(Vector2(horizontal, 0) * Time::deltaTime() * runSpeed);

		if (!Camera::cameras().empty())
			Camera::cameras()[0]->position = actor()->position;
	}
	
	json TestBehaviour::serialize()
	{
		json j = Behaviour::serialize();
		j["typeID"] = TRISTEON_TYPENAME(TestBehaviour);
		j["jumpVelocity"] = jumpVelocity;
		j["groundedDistance"] = groundedDistance;
		j["runSpeed"] = runSpeed;
		return j;
	}

	void TestBehaviour::deserialize(json j)
	{
		Behaviour::deserialize(j);
		
		jumpVelocity = j.value("jumpVelocity", 1500);
		groundedDistance = j.value("groundedDistance", 256);
		runSpeed = j.value("runSpeed", 100);
	}

	bool TestBehaviour::preContact(Contact const& contact)
	{
		if (contact.other->actor()->name == "A")
		{
			const auto box = dynamic_cast<BoxCollider*>(contact.other);

			if (actor()->position.y < (contact.other->actor()->position.y + box->height()))
				return false;
		}

		return true;
	}
}