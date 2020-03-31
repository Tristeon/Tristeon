#pragma once
#include <box2d/b2_world_callbacks.h>

#include <Math/Vector2.h>
#include <Physics/Collider.h>

namespace Tristeon
{
	class RaycastResult : public b2RayCastCallback
	{
	public:
		Collider* collider = nullptr;
		Vector2 point;
		Vector2 normal;

		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;
	};
}