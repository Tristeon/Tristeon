#pragma once
#include <box2d/b2_world.h>
#include <TypeDefinitions.h>
#include <Utils/Singleton.h>
#include <Math/Vector2.h>

namespace Tristeon
{
	class RaycastResult;

	class PhysicsWorld : Singleton<PhysicsWorld>
	{
		friend class Collider;
		friend class Rigidbody;
		friend class PhysicsBody;
		friend class Engine;
		
	public:
		PhysicsWorld();

		//TODO: pixel to meter conversion should be configurable through project physics settings
		static float pixelsToMeters(uint32 const& pixels);
		static Vector2 pixelsToMeters(Vector2 const& pixels);

		static uint32 metersToPixels(float const& meters);
		static Vector2 metersToPixels(Vector2 const& meters);

		static bool raycast(Vector2 const& origin, Vector2 const& direction, float const& distance);
		static bool raycast(Vector2 const& origin, Vector2 const& direction, float const& distance, RaycastResult& result);
	private:
		void update();

		int velocityIterations = 8;
		int positionIterations = 3;
		
		Unique<b2World> world;
	};
}
