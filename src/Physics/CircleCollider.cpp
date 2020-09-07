#include "CircleCollider.h"
#include <box2d/b2_shape.h>

#include "Actors/Actor.h"
#include "box2d/b2_circle_shape.h"
#include "PhysicsWorld.h"
#include "Rendering/Gizmos.h"

namespace Tristeon
{
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
		_radius = j.value("radius", PhysicsWorld::metersToPixels(1));
	}

	void CircleCollider::drawGizmos()
	{
		Gizmos::drawCircle(actor()->position, radius(), Colour{ 46 / 255.0f, 204 / 255.0f, 113 / 255.0f });
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

		circle->m_radius = PhysicsWorld::pixelsToMeters(_radius * std::max(actor()->scale.x, actor()->scale.y));

		if (includeBodyTransform)
			circle->m_p = PhysicsWorld::pixelsToMeters(actor()->position + _offset).convert<b2Vec2>();
		else
			circle->m_p = PhysicsWorld::pixelsToMeters(_offset).convert<b2Vec2>();

		shape = std::unique_ptr<b2Shape>(circle);
	}
}
