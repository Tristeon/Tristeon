#include "PhysicsWorld.h"

#include <Physics/RaycastResult.h>

#include "Project.h"

namespace Tristeon
{
	PhysicsWorld::PhysicsWorld()
	{
		b2Vec2 gravity(0.0f, -10.0f);
		_world = std::make_unique<b2World>(gravity);
		_world->SetContactListener(&_listener);

		b2BodyDef bodyDef;
		bodyDef.position = b2Vec2(0, 0);
		bodyDef.angle = 0.0f;
		bodyDef.type = b2_staticBody;
		bodyDef.fixedRotation = true;
		
		_staticBody = std::unique_ptr<b2Body, PhysicsBody::BodyDeleter>(_world->CreateBody(&bodyDef), {});
	}

	void PhysicsWorld::update()
	{
		_world->Step(Project::Physics::fixedDeltaTime() / 1000.0f, _velocityIterations, _positionIterations);
	}

	float PhysicsWorld::pixelsToMeters(const float& pixels)
	{
		return pixels / (float)Project::Physics::pixelsPerMeter();
	}

	Vector PhysicsWorld::pixelsToMeters(const Vector& pixels)
	{
		return pixels / (float)Project::Physics::pixelsPerMeter();
	}

	float PhysicsWorld::metersToPixels(const float& meters)
	{
		return meters * (float)Project::Physics::pixelsPerMeter();
	}

	Vector PhysicsWorld::metersToPixels(const Vector& meters)
	{
		return meters * (float)Project::Physics::pixelsPerMeter();
	}

	bool PhysicsWorld::raycast(const Vector& origin, const Vector& direction, const float& distance)
	{
		if (distance == 0)
			return false;
		
		auto* world = instance()->_world.get();

		RaycastResult callback;
		auto const point1 = pixelsToMeters(origin);
		auto const point2 = pixelsToMeters(origin + Vector(direction).normalize() * distance);
		world->RayCast(&callback, { point1.x, point1.y }, { point2.x, point2.y });

		return callback.collider != nullptr;
	}

	bool PhysicsWorld::raycast(const Vector& origin, const Vector& direction, const float& distance,
		RaycastResult& result)
	{
		if (distance == 0)
			return false;
		
		auto* world = instance()->_world.get();

		auto const point1 = pixelsToMeters(origin);
		auto const point2 = pixelsToMeters(origin + direction * distance);
		world->RayCast(&result, { point1.x, point1.y }, { point2.x, point2.y });

		return result.collider != nullptr;
	}
}