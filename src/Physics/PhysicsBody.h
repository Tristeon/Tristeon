#pragma once
#include <Scenes/Actors/Behaviour.h>

#include <memory>
#include <box2d/b2_body.h>
#include <Math/Vector.h>

#include <Serialization/TypeRegister.h>

#include <Callbacks/IStart.h>
#include <Callbacks/IFixedUpdate.h>
#include <Callbacks/IPreDestroy.h>

namespace Tristeon
{
	/**
	 * PhysicsBody describes a 2D body that simulates a physically "real" object.
	 * A physics body can not be bent, squished or in any other way deformed. The physics simulation only adjusts its position and rotation.
	 *
	 * The PhysicsBody's behaviour can be adjusted by applying forces, adjusting parameters or by directly changing its position/rotation values.
	 *
	 * Collider uses the Box2D API and as such all coordinates and sizes are internally converted to meters.
	 */
	class PhysicsBody : public Behaviour, public IFixedUpdate, public IStart, public IPreDestroy
	{
		friend class Collider;
		friend class SceneManager;
		friend struct RaycastResult;
		friend class CollisionListener;

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
		enum BodyType
		{
			/**
			 * Used for non-moving colliders, although this is usually unnecessary as Tristeon resolves static colliders without manually added PhysicsBodies.
			 */
			Static = b2BodyType::b2_staticBody,
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
		void applyForce(const Vector& force, const ForceMode& mode = ForceMode::Force);

		/**
		 * Adds a force, applied at the given global point. If the force is not applied at the center of mass, it will generate a torque and affect the angular velocity.
		 */
		void applyForceToPoint(const Vector& force, const Vector& point, const ForceMode& mode = ForceMode::Force);

		/**
		 * Apply a torque. This solely affects the angular velocity.
		 */
		void applyTorque(const float& torque);

		/**
		 * Gets the velocity in pixels.
		 */
		[[nodiscard]] Vector velocity() const;
		/**
		 * Sets the velocity to the given value.
		 */
		void setVelocity(const Vector& value);
		/**
		 * Sets the velocity to the given value.
		 */
		void setVelocity(const float& x, const float& y);
		/**
		 * Sets the x value of the velocity to the given value.
		 */
		void setVelocityX(const float& value);
		/**
		 * Sets the y value of the velocity to the given value.
		 */
		void setVelocityY(const float& value);
		
		/**
		 * Gets the gravity scale. This is a multiplier, defaulted to 1.
		 */
		[[nodiscard]] float gravityScale() const;
		/**
		 * Sets the gravity scale.
		 */
		void setGravityScale(const float& value);

		/**
		 * Gets whether or not the rotation of the PhysicsBody is fixed.
		 *
		 * If true, the PhysicsBody doesn't adjust Actor::rotation at all and angular forces do not apply.
		 */
		[[nodiscard]] bool fixedRotation() const;
		/**
		 * Sets whether or not the rotation of the PhysicsBody is fixed.
		 */
		void setFixedRotation(const bool& value);

		/**
		 * Gets the linear damping. Linear damping is used to reduce the linear velocity. Usually between 0.0f and 1.0f.
		 */
		[[nodiscard]] float linearDamping() const;
		/**
		 * Sets the linear damping.
		 */
		void setLinearDamping(const float& value);

		/**
		 * Gets the angular damping. Angular damping is used to reduce the angular velocity. Usually between 0.0f an 1.0f.
		 */
		[[nodiscard]] float angularDamping() const;
		/**
		 * Sets the angular damping.
		 */
		void setAngularDamping(const float& value);

		/**
		 * Gets the position in pixels.
		 */
		[[nodiscard]] Vector position() const;
		/**
		 * Sets the position.
		 */
		void setPosition(const Vector& value);

		/**
		 * The rotation in degrees.
		 */
		[[nodiscard]] float rotation() const;
		/**
		 * Sets the rotation.
		 */
		void setRotation(const float& value);

		/**
		 * Gets if the body has enabled continuous collision detection.
		 * This is useful for high-speed objects such as bullets, but increases performance cost so use it sparingly.
		 */
		[[nodiscard]] bool continuous() const;
		/**
		 * Enables/Disables continuous collision detection.
		 */
		void setContinuous(const bool& value);

		/**
		 * Gets if the body is enabled/simulated.
		 *
		 * If false, the body does not simulate any sort of physics.
		 */
		[[nodiscard]] bool enabled() const;
		/**
		 * Enables/Disables the body.
		 */
		void setEnabled(const bool& value);

		/**
		 * Gets the body type (Static, Kinematic, Dynamic).
		 */
		[[nodiscard]] BodyType type() const;
		/**
		 * Sets the body type (Static, Kinematic, Dynamic).
		 */
		void setType(const BodyType& type);
		
	protected:
		BodyType _type = Dynamic;

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

	REGISTER_TYPE(PhysicsBody);
}
