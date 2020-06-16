#include <Actors/Behaviours/TestBehaviour.h>
#include <Actors/Actor.h>

#include <Input/Gamepad.h>
#include <Input/Keyboard.h>
#include <Physics/PhysicsBody.h>

#include "Actors/Sprite.h"
#include "Physics/BoxCollider.h"
#include "Physics/Collider.h"
#include "Physics/PhysicsWorld.h"
#include "Rendering/Gizmos.h"
#include "Scenes/Camera.h"
#include "Utils/Time.h"

namespace Tristeon
{
	REGISTER_BEHAVIOUR_CPP(TestBehaviour)
	REGISTER_TYPE_CPP(TestBehaviour)

	void TestBehaviour::start()
	{
		body = getOwner<Actor>()->getBehaviour<PhysicsBody>();
	}

	void TestBehaviour::update()
	{
		const auto grounded = PhysicsWorld::raycast(getOwner<Actor>()->position, Vector2::down(), groundedDistance);
		if (Keyboard::pressed(Keyboard::Space) && grounded)
			body->setVelocity({ body->velocity().x, jumpVelocity });

		float const horizontal = Keyboard::held(Keyboard::D) - Keyboard::held(Keyboard::A);
		body->applyForce(Vector2(horizontal, 0) * Time::deltaTime() * runSpeed);

		Camera::main()->position = static_cast<Vector2Int>(getOwner<Actor>()->position);
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

	bool TestBehaviour::preContact(Contact const& contact)
	{
		if (contact.other->getOwner()->name == "A")
		{
			const auto box = dynamic_cast<BoxCollider*>(contact.other);

			if (getOwner()->position.y < (contact.other->getOwner()->position.y + box->height()))
				return false;
		}

		return true;
	}
}