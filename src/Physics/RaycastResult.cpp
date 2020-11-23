#include "RaycastResult.h"

#include <Physics/PhysicsBody.h>
#include <Physics/PhysicsWorld.h>

namespace Tristeon
{
	float RaycastResult::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
	{
		this->collider = (Collider*)(void*)fixture->GetUserData().pointer;
		this->point = PhysicsWorld::metersToPixels({ point.x, point.y });
		this->normal = { normal.x, normal.y };
		
		return fraction;
	}
}