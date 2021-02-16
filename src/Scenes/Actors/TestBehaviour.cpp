#include <Scenes/Actors/TestBehaviour.h>
#include <Scenes/Actors/Actor.h>

#include <Input/Gamepad.h>
#include <Input/Keyboard.h>
#include <Physics/PhysicsBody.h>


#include "Settings.h"
#include "Audio/Audio.h"
#include "Rendering/Camera.h"
#include "Physics/BoxCollider.h"
#include "Physics/Collider.h"
#include "Physics/PhysicsWorld.h"
#include "Rendering/Gizmos.h"
#include "Scenes/SceneManager.h"
#include "Utils/Time.h"

namespace Tristeon
{
	void TestBehaviour::init()
	{
		body = actor()->findBehaviour<PhysicsBody>();
	}

	void TestBehaviour::update()
	{
		if (!body)
			return;
		
		const auto grounded = PhysicsWorld::raycast(actor()->position, Vector::down(), groundedDistance);
		if (Keyboard::pressed(Keyboard::Space) && grounded)
			body->setVelocity({ body->velocity().x, jumpVelocity });

		float const horizontal = (float)(Keyboard::held(Keyboard::D) - Keyboard::held(Keyboard::A));
		body->applyForce(Vector(horizontal, 0) * Time::deltaTime() * runSpeed);

		if (!Camera::cameras().empty())
			Camera::cameras()[0]->position = actor()->position;

		if (Keyboard::pressed(Keyboard::Number1))
			Settings::Graphics::setPreferredDisplay(0);
		else if (Keyboard::pressed(Keyboard::Number2))
			Settings::Graphics::setPreferredDisplay(1);
		else if (Keyboard::pressed(Keyboard::Number3))
			Settings::Graphics::setPreferredDisplay(2);
	}
	
	json TestBehaviour::serialize()
	{
		json j = Behaviour::serialize();
		j["typeID"] = Type<TestBehaviour>::fullName();
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