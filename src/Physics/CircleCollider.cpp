#include "CircleCollider.h"
#include <box2d/b2_shape.h>
#include "box2d/b2_circle_shape.h"
#include "PhysicsWorld.h"

namespace Tristeon
{
	REGISTER_TYPE_CPP(CircleCollider)

	json CircleCollider::serialize()
	{
		json j = Collider::serialize();
		j["typeID"] = TRISTEON_TYPENAME(CircleCollider);
		j["radius"] = _radius;
		return j;
	}

	void CircleCollider::deserialize(json j)
	{
		Collider::deserialize(j);
		_radius = j["radius"];
	}

	float CircleCollider::radius() const
	{
		return _radius;
	}

	void CircleCollider::radius(float const& value)
	{
		_radius = value;
		isDirty = true;
	}

	void CircleCollider::createShape()
	{
		auto* circle = new b2CircleShape();
		circle->m_radius = PhysicsWorld::pixelsToMeters(_radius);
		circle->m_p = PhysicsWorld::pixelsToMeters(_offset).convert<b2Vec2>();
		shape = std::unique_ptr<b2Shape>(circle);
	}
}