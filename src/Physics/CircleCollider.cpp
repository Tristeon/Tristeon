#include "CircleCollider.h"
#include <box2d/b2_shape.h>

#include "Scenes/Actors/Actor.h"
#include "box2d/b2_circle_shape.h"
#include "PhysicsWorld.h"
#include "Rendering/Gizmos.h"

namespace Tristeon
{
	json CircleCollider::serialize()
	{
		json j = Collider::serialize();
		j["typeID"] = Type<CircleCollider>::fullName();
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

	void CircleCollider::setRadius(const float& value)
	{
		_radius = value;
		_isDirty = true;
	}

	void CircleCollider::createShape(const bool& includeBodyTransform)
	{
		auto* circle = new b2CircleShape();

		circle->m_radius = PhysicsWorld::pixelsToMeters(_radius * std::max(actor()->scale.x, actor()->scale.y));

		if (includeBodyTransform)
		{
			const auto p = PhysicsWorld::pixelsToMeters(actor()->position + _offset);
			circle->m_p = b2Vec2(p.x, p.y);
		}
		else
		{
			const auto p = PhysicsWorld::pixelsToMeters(_offset);
			circle->m_p = b2Vec2(p.x, p.y);
		}

		_shape = std::unique_ptr<b2Shape>(circle);
	}
}