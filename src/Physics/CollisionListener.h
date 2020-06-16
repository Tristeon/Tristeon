#pragma once
#include "box2d/b2_world_callbacks.h"

namespace Tristeon
{
	/**
	 * Internal class. Used to listen to box2D collision callbacks and relay them to I..Begin/End callbacks.
	 * Do not use.
	 */
	class CollisionListener : public b2ContactListener
	{
	public:
		void BeginContact(b2Contact* contact) override;
		void EndContact(b2Contact* contact) override;

		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
	};
}