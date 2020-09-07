#pragma once
#include <box2d/b2_world_callbacks.h>

#include <Math/Vector2.h>
#include <Physics/Collider.h>

namespace Tristeon
{
	/**
	 * The resulting value of a raycast.
	 * This structure may be empty, meaning collider == nullptr, point = { 0, 0 } and normal = { 0, 0 }.
	 * This is the case when a raycast fails to hit a target.
	 */
	struct RaycastResult : b2RayCastCallback
	{
		/**
		 * The collider hit by the raycast, nullptr if no collider was hit.
		 */
		Collider* collider = nullptr;
		
		/**
		 * The exact point at which the raycast hit the collider.
		 */
		Vector2 point{};
		
		/**
		 * The normal at the point of intersection.
		 */
		Vector2 normal{};

		/**
		 * ReportFixture is internally used by box2D to receive raycast callbacks.
		 */
		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;
	};
}