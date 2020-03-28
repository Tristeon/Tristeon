#include "Collider.h"

#include <Actors/Actor.h>

#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

#include <Physics/PhysicsBody.h>
#include "Math/Math.h"
#include "PhysicsWorld.h"

namespace Tristeon
{
	REGISTER_TYPE_CPP(Collider)

	Collider::~Collider()
	{
		if (owner() != nullptr && owner()->behaviour<PhysicsBody>() != nullptr)
			owner()->behaviour<PhysicsBody>()->remove(this);
	}

	void Collider::start()
	{
		if (!owner()->behaviour<PhysicsBody>())
			throw std::runtime_error("Can't have a collider without a PhysicsBody!");
		else
			owner()->behaviour<PhysicsBody>()->add(this);
	}

	void Collider::lateUpdate()
	{
		if (isDirty)
		{
			PhysicsBody* body = owner()->behaviour<PhysicsBody>();
			body->resetCollider(this);
		}
	}

	json Collider::serialize()
	{
		json j;
		j["typeID"] = TRISTEON_TYPENAME(Collider);

		j["width"] = _width;
		j["height"] = _height;
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
		_width = j["width"];
		_height = j["height"];
		_offset = j["offset"];
		_rotationOffset = j["rotationOffset"];
		
		_density = j["density"];
		_friction = j["friction"];
		_restitution = j["restitution"];
		
		_sensor = j["sensor"];
		
		createShape(); //Re-create shape
	}

	float Collider::width() const
	{
		return _width;
	}

	void Collider::width(float const& value)
	{
		_width = value;
		isDirty = true;
	}

	float Collider::height() const
	{
		return _height;
	}

	void Collider::height(float const& value)
	{
		_height = value;
		isDirty = true;
	}

	float Collider::density() const
	{
		return _density;
	}

	void Collider::density(float const& value)
	{
		_density = value;

		b2Fixture* fixture = getFixture();
		if (fixture != nullptr)
			fixture->SetDensity(value);
	}

	float Collider::friction() const
	{
		return _friction;
	}

	void Collider::friction(float const& value)
	{
		_friction = value;

		b2Fixture* fixture = getFixture();
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

		b2Fixture* fixture = getFixture();
		if (fixture != nullptr)
			fixture->SetRestitution(value);
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

	bool Collider::sensor() const
	{
		return _sensor;
	}

	void Collider::sensor(bool const& value)
	{
		_sensor = value;
		b2Fixture* fixture = getFixture();
		if (fixture != nullptr)
			fixture->SetSensor(value);
	}

	void Collider::createShape()
	{
		auto* polygon = new b2PolygonShape();

		Vector2 const size = PhysicsWorld::pixelsToMeters({ _width, _height });
		Vector2 const offset = PhysicsWorld::pixelsToMeters(_offset);
		polygon->SetAsBox(size.x / 2.0f, size.y / 2.0f, offset.convert<b2Vec2>(), Math::toRadians(_rotationOffset));
		shape = std::unique_ptr<b2Shape>(polygon);
	}

	b2Fixture* Collider::getFixture()
	{
		return owner()->behaviour<PhysicsBody>()->fixtures[this];
	}

	b2Shape* Collider::getShape()
	{
		if (shape == nullptr)
			createShape();
		return shape.get();
	}
}