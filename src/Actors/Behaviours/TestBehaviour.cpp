#include <Actors/Behaviours/TestBehaviour.h>
#include <Actors/Actor.h>

#include <Input/Gamepad.h>
#include <Input/Keyboard.h>

#include <Rendering/GameView.h>

#include <Physics/PhysicsBody.h>


#include "Input/Mouse.h"
#include "Math/Math.h"
#include "Physics/Collider.h"
#include "Physics/PhysicsWorld.h"
#include "Scenes/Camera.h"

namespace Tristeon
{
	REGISTER_BEHAVIOUR_CPP(TestBehaviour)
	REGISTER_TYPE_CPP(TestBehaviour)

	void TestBehaviour::start()
	{
		body = getOwner<Actor>()->getBehaviour<PhysicsBody>();
	}

	int r = 0;
	
	void TestBehaviour::update()
	{
		getOwner<Actor>()->position = Math::orbit(GameView::screenToWorld(Mouse::position()), Vector2{ 500, 500 }, r++);
		//owner()->position = GameView::screenToWorld(Mouse::position());
		//std::cout
		//	<< "Mouse: " << Mouse::position().toString() << std::endl
		//	<< "Mouse to world: " << GameView::screenToWorld(Mouse::position()).toString() << std::endl
		//	<< "World to mouse: " << GameView::worldToScreen(GameView::screenToWorld(Mouse::position())).toString() << std::endl;
		return;
		
		bool grounded = PhysicsWorld::raycast(getOwner<Actor>()->position, Vector2::down(), groundedDistance);
		if (Keyboard::pressed(Keyboard::Space) && grounded)
			body->setVelocity({ body->velocity().x, jumpVelocity });

		float const horizontal = Keyboard::held(Keyboard::D) - Keyboard::held(Keyboard::A);
		body->applyForce(Vector2(horizontal, 0) * GameView::deltaTime() * runSpeed);

		Camera::main()->position = (Vector2Int)getOwner<Actor>()->position;
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
	}

	void TestBehaviour::contactEnd(Collider* other)
	{
	}

	void TestBehaviour::tileContactBegin(TileContact const& contact)
	{
	}

	void TestBehaviour::tileContactEnd(TileContact const& contact)
	{
	}
}
