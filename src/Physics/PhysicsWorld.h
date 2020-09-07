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

		/**
		 * Converts pixel coordinates into box2D compatible meter coordinates.
		 *
		 * This is because box2D exclusively uses a MKS (meter, kilograms, seconds) system to define & calculate location, mass and time.
		 * Passing normal pixel coordinates to box2d will make box2d interpret each pixel as a full meter, causing physics to be off.
		 */
		[[nodiscard]] static float pixelsToMeters(const uint32& pixels);

		/**
		 * Converts pixel coordinates into box2D compatible meter coordinates.
		 *
		 * This is because box2D exclusively uses a MKS (meter, kilograms, seconds) system to define & calculate location, mass and time.
		 * Passing normal pixel coordinates to box2d will make box2d interpret each pixel as a full meter, causing physics to be off.
		 */
		[[nodiscard]] static Vector2 pixelsToMeters(const Vector2& pixels);

		/**
		 * Converts box2D meters back to Tristeon pixels.
		 * This is used to convert box2D's output back into Tristeon compatible coordinates.
		 */
		[[nodiscard]] static uint32 metersToPixels(const float& meters);

		/**
		 * Converts box2D meters back to Tristeon pixels.
		 * This is used to convert box2D's output back into Tristeon compatible coordinates.
		 */
		[[nodiscard]] static Vector2 metersToPixels(const Vector2& meters);

		/**
		 * Cast a ray from origin to origin + direction * distance. Stops at the first collider it finds and returns true there.
		 * If no collider was found along the ray then the function returns false.
		 *
		 * \param origin The starting point of the raycast.
		 * \param direction The direction of the raycast, this value is expected to be normalized.
		 * \param distance The length of the raycast in pixels.
		 */
		[[nodiscard]] static bool raycast(const Vector2& origin, const Vector2& direction, const float& distance);
		/**
		 * Cast a ray from origin to origin + direction * distance. Stops at the first collider it finds and returns true there.
		 * If no collider was found along the ray then the function returns false.
		 *
		 * \param origin The starting point of the raycast.
		 * \param direction The direction of the raycast, this value is expected to be normalized.
		 * \param distance The length of the raycast in pixels.
		 * \param result Empty if no collider was hit, otherwise it will contain info describing the raycast hit.
		 */
		[[nodiscard]] static bool raycast(const Vector2& origin, const Vector2& direction, const float& distance, RaycastResult& result);
	private:
		void update();

		//TODO: Move velocity & position iterations to the project settings
		int _velocityIterations = 8;
		int _positionIterations = 3;
		
		Unique<b2World> _world = nullptr;

		Unique<b2Body, PhysicsBody::BodyDeleter> _staticBody = nullptr;
		std::map<Collider*, b2Fixture*> _fixtures{};
		
		CollisionListener _listener{};
	};
}
