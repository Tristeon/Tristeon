#include "CircleCollider.h"
#include <box2d/b2_shape.h>

#include "Actors/Actor.h"
#include "box2d/b2_circle_shape.h"
#include "PhysicsWorld.h"

namespace Tristeon
{
	REGISTER_BEHAVIOUR_CPP(CircleCollider)
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

	void CircleCollider::createShape(bool const& includeBodyTransform)
	{
		auto* circle = new b2CircleShape();

		circle->m_radius = PhysicsWorld::pixelsToMeters(_radius * std::max(getOwner()->scale.x, getOwner()->scale.y));
		
		if (includeBodyTransform)
			circle->m_p = PhysicsWorld::pixelsToMeters(getOwner()->position + _offset).convert<b2Vec2>();
		else
			circle->m_p = PhysicsWorld::pixelsToMeters(_offset).convert<b2Vec2>();

		shape = std::unique_ptr<b2Shape>(circle);
	}
}