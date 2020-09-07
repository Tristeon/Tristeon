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
		_lastScale = actor()->scale;
		
		removeSelf();
		if (_body == nullptr)
		{
			PhysicsBody* pb = actor()->behaviour<PhysicsBody>();
			if (pb != nullptr)
				_body = pb->getBody();
			else
				_body = PhysicsWorld::instance()->_staticBody.get();
		}
		addSelf();
	}

	void Collider::lateUpdate()
	{
		//Check if the actor's scale has been changed, rebuild collider if that's the case.
		if (_lastScale != actor()->scale)
		{
			_isDirty = true;
			_lastScale = actor()->scale;
		}
		
		if (_isDirty)
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
		json j = Behaviour::serialize();
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
		Behaviour::deserialize(j);
		
		_offset = j.value("offset", Vector2::zero());
		_rotationOffset = j.value("rotationOffset", 0);
		
		_density = j.value("density", 1.0f);
		_friction = j.value("friction", 0.0f);
		_restitution = j.value("restitution", 0.0f);
		
		_sensor = j.value("sensor", false);
	}

	float Collider::density() const
	{
		return _density;
	}

	void Collider::setDensity(float const& value)
	{
		_density = value;

		if (_fixture != nullptr)
		{
			_fixture->SetDensity(value);
			_fixture->GetBody()->ResetMassData();
		}
	}

	float Collider::friction() const
	{
		return _friction;
	}

	void Collider::setFriction(float const& value)
	{
		_friction = value;

		if (_fixture != nullptr)
			_fixture->SetFriction(value);
	}

	float Collider::restitution() const
	{
		return _restitution;
	}

	void Collider::setRestitution(float const& value)
	{
		_restitution = value;

		if (_fixture != nullptr)
			_fixture->SetRestitution(value);
	}

	bool Collider::sensor() const
	{
		return _sensor;
	}

	void Collider::setSensor(bool const& value)
	{
		_sensor = value;
		if (_fixture != nullptr)
			_fixture->SetSensor(value);
	}

	b2Shape* Collider::getShape(bool const& includeOwnerTransform)
	{
		createShape(includeOwnerTransform);
		return _shape.get();
	}

	void Collider::addSelf()
	{
		//Either find a body or get the default static one
		if (_body == nullptr)
		{
			PhysicsBody* pb = actor()->behaviour<PhysicsBody>();
			if (pb != nullptr)
				_body = pb->getBody();
			else
				_body = PhysicsWorld::instance()->_staticBody.get();
		}

		//Create the collider fixture
		b2FixtureDef def;
		def.shape = getShape(_body->GetType() == b2_staticBody);
		def.density = density();
		def.friction = friction();
		def.isSensor = sensor();
		def.restitution = restitution();

		_fixture = _body->CreateFixture(&def);
		_fixture->SetUserData(this);
	}

	void Collider::removeSelf()
	{
		if (_body == nullptr)
			return;
		
		_body->DestroyFixture(_fixture);
		_fixture = nullptr;
		_body = nullptr;
	}

	void Collider::setPhysicsBody(b2Body* newBody)
	{
		if (_body == nullptr)
			return;
		
		removeSelf();
		_body = newBody;
		if (_body == nullptr)
			_body = PhysicsWorld::instance()->_staticBody.get();
		addSelf();
	}

	Vector2 Collider::offset() const
	{
		return _offset;
	}

	void Collider::setOffset(Vector2 const& value)
	{
		_offset = value;
		_isDirty = true;
	}

	float Collider::rotationOffset() const
	{
		return _rotationOffset;
	}

	void Collider::setRotationOffset(float const& value)
	{
		_rotationOffset = value;
		_isDirty = true;
	}
}
