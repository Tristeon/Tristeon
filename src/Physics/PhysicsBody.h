#pragma once
#include <Actors/Behaviours/Behaviour.h>

#include <memory>
#include <box2d/b2_body.h>
#include <Math/Vector2.h>

#include <Serialization/TypeRegister.h>

#include <Callbacks/IStart.h>
#include <Callbacks/IFixedUpdate.h>
#include <Callbacks/IPreDestroy.h>

#include <Registers/BehaviourRegister.h>

namespace Tristeon
{
	/**
	 * PhysicsBody describes a 2D body that simulates a physically "real" object.
	 * A physics body can not be bent, squished or in any other way deformed. The physics simulation only adjusts its position and rotation.
	 *
	 * The PhysicsBody's behaviour can be adjusted by applying forces, adjusting parameters or by directly changing its position/rotation values.
	 */
	class PhysicsBody : public Behaviour, public IFixedUpdate, public IStart, public IPreDestroy
	{
		friend class Collider;
		friend class SceneManager;
		friend struct RaycastResult;
		friend class CollisionListener;

		REGISTER_BEHAVIOUR_H(PhysicsBody)
		REGISTER_TYPE_H(PhysicsBody)
	public:
		json serialize() override;
		void deserialize(json j) override;
		
		void start() override;
		void fixedUpdate() override;
		void preDestroy() override;

		struct BodyDeleter
		{
			void operator()(b2Body* body);
		};

		/**
		 * Describes the PhysicsBody's type; the way it interacts with the world.
		 */
		enum Type
		{
			/**
			 * A kinematic PhysicsBody does not get affected by forces, collisions or joints, but unlike a
			 * plain static collider, it can be moved around freely.
			 *
			 * This can be helpful for things such as moving platforms, as they should always stay on their track no matter how they're being jumped on or collided with.
			 */
			Kinematic = b2BodyType::b2_kinematicBody,
			/**
			 * A dynamic PhysicsBody interacts with the world in a Physically accurate way. It's affected by gravity, collision forces, etc.
			 */
			Dynamic = b2BodyType::b2_dynamicBody
		};

		/**
		 * The type of force applied to the PhysicsBody using any of the applyForce() functions.
		 */
		enum ForceMode
		{
			/**
			 * A continuous force
			 */
			Force,
			/**
			 * An instant force
			 */
			Impulse,
		};

		/**
		 * Adds a force, applied at the center of mass of the body.
		 */
		void applyForce(Vector2 const& force, ForceMode const& mode = ForceMode::Force);

		/**
		 * Adds a force, applied at the given global point. If the force is not applied at the center of mass, it will generate a torque and affect the angular velocity.
		 */
		void applyForceToPoint(Vector2 const& force, Vector2 const& point, ForceMode const& mode = ForceMode::Force);

		/**
		 * Apply a torque. This solely affects the angular velocity.
		 */
		void applyTorque(float const& torque);

		/**
		 * Gets the velocity in pixels.
		 */
		Vector2 velocity() const;
		/**
		 * Sets the velocity to the given value.
		 */
		void setVelocity(Vector2 const& value);
		/**
		 * Sets the velocity to the given value.
		 */
		void setVelocity(float const& x, float const& y);

		/**
		 * Gets the gravity scale. This is a multiplier, defaulted to 1.
		 */
		float gravityScale() const;
		/**
		 * Sets the gravity scale.
		 */
		void setGravityScale(float const& value);

		/**
		 * Gets whether or not the rotation of the PhysicsBody is fixed.
		 *
		 * If true, the PhysicsBody doesn't adjust Actor::rotation at all and angular forces do not apply.
		 */
		bool fixedRotation() const;
		/**
		 * Sets whether or not the rotation of the PhysicsBody is fixed.
		 */
		void setFixedRotation(bool const& value);

		/**
		 * Gets the linear damping. Linear damping is used to reduce the linear velocity. Usually between 0.0f and 1.0f.
		 */
		float linearDamping() const;
		/**
		 * Sets the linear damping.
		 */
		void setLinearDamping(float const& value);

		/**
		 * Gets the angular damping. Angular damping is used to reduce the angular velocity. Usually between 0.0f an 1.0f.
		 */
		float angularDamping() const;
		/**
		 * Sets the angular damping.
		 */
		void setAngularDamping(float const& value);

		/**
		 * Gets the position in pixels.
		 */
		Vector2 position() const;
		/**
		 * Sets the position.
		 */
		void setPosition(Vector2 const& value);

		/**
		 * The rotation in degrees.
		 */
		float rotation() const;
		/**
		 * Sets the rotation.
		 */
		void setRotation(float const& value);

		/**
		 * Gets if the body has enabled continuous collision detection.
		 * This is useful for high-speed objects such as bullets, but increases performance cost so use it sparingly.
		 */
		bool continuous() const;
		/**
		 * Enables/Disables continuous collision detection.
		 */
		void setContinuous(bool const& value);

		/**
		 * Gets if the body is enabled/simulated.
		 *
		 * If false, the body does not simulate any sort of physics.
		 */
		bool enabled() const;
		/**
		 * Enables/Disables the body.
		 */
		void setEnabled(bool const& value);
		
	protected:
		Type type = Dynamic;

		float _gravityScale = 1;
		bool _fixedRotation = false;

		float _linearDamping = 0.0f;
		float _angularDamping = 0.01f;

		bool _enabled = true;

		bool _continuous = false;

		//TODO: Handle Dirty colliders
		//TODO: Handle removal and addition of colliders
		std::unique_ptr<b2Body, BodyDeleter> body;

		b2Body* getBody();
		void createBody();
	};
}
