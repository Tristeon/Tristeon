#include "BoxCollider.h"

#include <Math/Math.h>
#include <Physics/PhysicsWorld.h>

#include <box2d/b2_polygon_shape.h>

#include "Scenes/Actors/Actor.h"
#include "Rendering/Gizmos.h"

namespace Tristeon
{
	void BoxCollider::createShape(const bool& includeBodyTransform)
	{
		auto* polygon = new b2PolygonShape();

		Vector2 const size = PhysicsWorld::pixelsToMeters({
			_width * actor()->scale.x, _height * actor()->scale.y
		});
		Vector2 offset = PhysicsWorld::pixelsToMeters(_offset);
		
		float rotationOffset = Math::toRadians(_rotationOffset);
		if (includeBodyTransform)
		{
			offset = PhysicsWorld::pixelsToMeters(actor()->position) + offset;
			rotationOffset = Math::toRadians(actor()->rotation) + rotationOffset;
		}

		polygon->SetAsBox(size.x / 2.0f, size.y / 2.0f, offset.convert<b2Vec2>(), -rotationOffset);
		_shape = std::unique_ptr<b2Shape>(polygon);
	}

	json BoxCollider::serialize()
	{
		json j = Collider::serialize();
		j["typeID"] = TRISTEON_TYPENAME(BoxCollider);
		j["width"] = _width;
		j["height"] = _height;
		return j;
	}

	void BoxCollider::deserialize(json j)
	{
		Collider::deserialize(j);

		_width = j.value("width", PhysicsWorld::metersToPixels(1));
		_height = j.value("height", PhysicsWorld::metersToPixels(1));
	}

	void BoxCollider::drawGizmos()
	{
		Gizmos::drawSquare(actor()->position, Vector2(width(), height()), actor()->rotation, {46 / 255.0f, 204 / 255.0f, 113 / 255.0f});
	}

	float BoxCollider::width() const
	{
		return _width;
	}

	void BoxCollider::setWidth(const float& value)
	{
		_width = value;
		_isDirty = true;
	}

	float BoxCollider::height() const
	{
		return _height;
	}

	void BoxCollider::setHeight(const float& value)
	{
		_height = value;
		_isDirty = true;
	}
}