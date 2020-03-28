#include "BoxCollider.h"

#include <Math/Math.h>
#include <Physics/PhysicsWorld.h>

#include <box2d/b2_polygon_shape.h>

namespace Tristeon
{
	REGISTER_TYPE_CPP(BoxCollider)
	
	void BoxCollider::createShape()
	{
		auto* polygon = new b2PolygonShape();

		Vector2 const size = PhysicsWorld::pixelsToMeters({ _width, _height });
		Vector2 const offset = PhysicsWorld::pixelsToMeters(_offset);
		polygon->SetAsBox(size.x / 2.0f, size.y / 2.0f, offset.convert<b2Vec2>(), Math::toRadians(_rotationOffset));
		shape = std::unique_ptr<b2Shape>(polygon);
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

		_width = j["width"];
		_height = j["height"];
	}

	float BoxCollider::width() const
	{
		return _width;
	}

	void BoxCollider::width(float const& value)
	{
		_width = value;
		isDirty = true;
	}

	float BoxCollider::height() const
	{
		return _height;
	}

	void BoxCollider::height(float const& value)
	{
		_height = value;
		isDirty = true;
	}
}