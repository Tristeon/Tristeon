#include "Collider.h"

#include <Actors/Actor.h>

#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

#include <Physics/PhysicsBody.h>
#include <Physics/PhysicsWorld.h>

namespace Tristeon
{
	void Collider::start()
	{
		if (body == nullptr)
		{
			PhysicsBody* pb = getOwner()->getBehaviour<PhysicsBody>();
			if (pb != nullptr)
				body = pb->getBody();
			else
				body = PhysicsWorld::instance()->staticBody.get();
		}

		removeSelf();
		addSelf();
	}

	void Collider::lateUpdate()
	{
		if (isDirty)
		{
			removeSelf();
			addSelf();
		}
	}

	void Collider::preDestroy()
	{
		removeSelf();
	}

	json Collider::serialize()
	{
		json j;
		j["typeID"] = TRISTEON_TYPENAME(Collider);

		j["offset"] = _offset;
		j["rotationOffset"] = _rotationOffset;
		
		j["density"] = _density;
		j["friction"] = _friction;
		j["restitution"] = _restitution;
		
		j["sensor"] = _sensor;
		
		return j;
	}

	void Collider::deserialize(json j)
	{
		_offset = j["offset"];
		_rotationOffset = j["rotationOffset"];
		
		_density = j["density"];
		_friction = j["friction"];
		_restitution = j["restitution"];
		
		_sensor = j["sensor"];
	}

	float Collider::density() const
	{
		return _density;
	}

	void Collider::density(float const& value)
	{
		_density = value;

		if (fixture != nullptr)
		{
			fixture->SetDensity(value);
			fixture->GetBody()->ResetMassData();
		}
	}

	float Collider::friction() const
	{
		return _friction;
	}

	void Collider::friction(float const& value)
	{
		_friction = value;

		if (fixture != nullptr)
			fixture->SetFriction(value);
	}

	float Collider::restitution() const
	{
		return _restitution;
	}

	void Collider::restitution(float const& value)
	{
		_restitution = value;

		if (fixture != nullptr)
			fixture->SetRestitution(value);
	}

	bool Collider::sensor() const
	{
		return _sensor;
	}

	void Collider::sensor(bool const& value)
	{
		_sensor = value;
		if (fixture != nullptr)
			fixture->SetSensor(value);
	}

	b2Shape* Collider::getShape(bool const& includeOwnerTransform)
	{
		createShape(includeOwnerTransform);
		return shape.get();
	}

	void Collider::addSelf()
	{
		b2FixtureDef def;
		def.shape = getShape(body->GetType() == b2_staticBody);
		def.density = density();
		def.friction = friction();
		def.isSensor = sensor();
		def.restitution = restitution();

		fixture = body->CreateFixture(&def);
		fixture->SetUserData(this);
	}

	void Collider::removeSelf()
	{
		if (body == nullptr)
			return;
		
		body->DestroyFixture(fixture);
		fixture = nullptr;
	}

	void Collider::setPhysicsBody(b2Body* newBody)
	{
		removeSelf();
		body = newBody;
		if (body == nullptr)
			body = PhysicsWorld::instance()->staticBody.get();
		addSelf();
	}

	Vector2 Collider::offset() const
	{
		return _offset;
	}

	void Collider::offset(Vector2 const& value)
	{
		_offset = value;
		isDirty = true;
	}

	float Collider::rotationOffset() const
	{
		return _rotationOffset;
	}

	void Collider::rotationOffset(float const& value)
	{
		_rotationOffset = value;
		isDirty = true;
	}
}