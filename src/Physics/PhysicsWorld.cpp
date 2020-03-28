#include "PhysicsWorld.h"

#include "Rendering/GameView.h"

namespace Tristeon
{
	PhysicsWorld::PhysicsWorld()
	{
		b2Vec2 gravity(0.0f, -10.0f);
		world = std::make_unique<b2World>(gravity);
	}

	void PhysicsWorld::add(Collider* collider)
	{
		
	}

	void PhysicsWorld::remove(Collider* collider)
	{
		
	}

	void PhysicsWorld::add(Rigidbody* rigidbody)
	{
		
	}

	void PhysicsWorld::remove(Rigidbody* rigidbody)
	{
		
	}

	void PhysicsWorld::update()
	{
		world->Step(1.0f / 60.0f, velocityIterations, positionIterations);
	}

	float PhysicsWorld::pixelsToMeters(uint32 const& pixels)
	{
		return pixels / 256.0f; //256 pixels is 1 meter
	}

	Vector2 PhysicsWorld::pixelsToMeters(Vector2 const& pixels)
	{
		return pixels / 256.0f;
	}

	uint32 PhysicsWorld::metersToPixels(float const& meters)
	{
		return meters * 256;
	}

	Vector2 PhysicsWorld::metersToPixels(Vector2 const& meters)
	{
		return meters * 256;
	}
}
