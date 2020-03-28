#pragma once
#include <box2d/b2_world.h>
#include <TypeDefinitions.h>
#include <Utils/Singleton.h>
#include <Math/Vector2.h>

namespace Tristeon
{
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
		
	private:
		void add(Collider* collider);
		void remove(Collider* collider);

		void add(Rigidbody* rigidbody);
		void remove(Rigidbody* rigidbody);

		void update();

		int velocityIterations = 8;
		int positionIterations = 3;
		
		Unique<b2World> world;
	};
}
