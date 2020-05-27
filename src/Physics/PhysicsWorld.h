#pragma once
#include <box2d/b2_world.h>
#include <TypeDefinitions.h>
#include <Utils/Singleton.h>
#include <Math/Vector2.h>

#include <Physics/CollisionListener.h>
#include <Physics/PhysicsBody.h>

namespace Tristeon
{
	struct RaycastResult;

	/**
	 * The PhysicsWorld controls the realtime physics simulation in Tristeon, it consists out of PhysicsBodies and Colliders.
	 *
	 * It implements box2D and can be interacted with through static functions like raycast(),
	 * and through behaviours such as BoxCollider, CircleCollider and PhysicsBody.
	 */
	class PhysicsWorld : public Singleton<PhysicsWorld>
	{
		friend class Collider;
		friend class PhysicsBody;
		friend class Engine;
		friend class TileLayer;
		friend class SceneManager;
	public:
		/**
		 * PhysicsWorld is created once by Engine and shouldn't be created again.
		 */
		PhysicsWorld();

		//TODO: pixel to meter conversion should be configurable through project physics settings
		/**
		 * Converts pixel coordinates into box2D compatible meter coordinates.
		 *
		 * This is because box2D exclusively uses a MKS (meter, kilograms, seconds) system to define & calculate location, mass and time.
		 * Passing normal pixel coordinates to box2d will make box2d interpret each pixel as a full meter, causing physics to be off.
		 */
		static float pixelsToMeters(uint32 const& pixels);

		/**
		 * Converts pixel coordinates into box2D compatible meter coordinates.
		 *
		 * This is because box2D exclusively uses a MKS (meter, kilograms, seconds) system to define & calculate location, mass and time.
		 * Passing normal pixel coordinates to box2d will make box2d interpret each pixel as a full meter, causing physics to be off.
		 */
		static Vector2 pixelsToMeters(Vector2 const& pixels);

		/**
		 * Converts box2D meters back to Tristeon pixels.
		 * This is used to convert box2D's output back into Tristeon compatible coordinates.
		 */
		static uint32 metersToPixels(float const& meters);

		/**
		 * Converts box2D meters back to Tristeon pixels.
		 * This is used to convert box2D's output back into Tristeon compatible coordinates.
		 */
		static Vector2 metersToPixels(Vector2 const& meters);

		/**
		 * Cast a ray from origin to origin + direction * distance. Stops at the first collider it finds and returns true there.
		 * If no collider was found along the ray then the function returns false.
		 *
		 * \param origin The starting point of the raycast.
		 * \param direction The direction of the raycast, this value is expected to be normalized.
		 * \param distance The length of the raycast in pixels.
		 */
		static bool raycast(Vector2 const& origin, Vector2 const& direction, float const& distance);
		/**
		 * Cast a ray from origin to origin + direction * distance. Stops at the first collider it finds and returns true there.
		 * If no collider was found along the ray then the function returns false.
		 *
		 * \param origin The starting point of the raycast.
		 * \param direction The direction of the raycast, this value is expected to be normalized.
		 * \param distance The length of the raycast in pixels.
		 * \param result Empty if no collider was hit, otherwise it will contain info describing the raycast hit.
		 */
		static bool raycast(Vector2 const& origin, Vector2 const& direction, float const& distance, RaycastResult& result);
	private:
		void update();

		int velocityIterations = 8;
		int positionIterations = 3;
		
		Unique<b2World> world;

		Unique<b2Body, PhysicsBody::BodyDeleter> staticBody;
		std::map<Collider*, b2Fixture*> fixtures;
		
		CollisionListener listener;
	};
}
