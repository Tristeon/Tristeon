#pragma once
#include <Actors/Behaviours/Behaviour.h>
#include <Math/Vector2.h>

#include <box2d/b2_shape.h>
#include <box2d/b2_fixture.h>

#include <Callbacks/IStart.h>
#include <Callbacks/ILateUpdate.h>
#include <Callbacks/IPreDestroy.h>

namespace Tristeon
{
	/**
	 * Base class for 2D Physics shape definitions.
	 *
	 * If the instance of this class belongs to an actor without a PhysicsBody, it is expected to be static object and moving it may be performance intensive.
	 * If you'd like a moveable collider without physical interactions (forces, joints, etc), you may consider adding a physicsbody and setting its type to kinematic.
	 */
	class Collider : public Behaviour, public IStart, public ILateUpdate, public IPreDestroy
	{
		friend class PhysicsBody;
		friend class PhysicsWorld;
	public:
		virtual ~Collider() = default;

		void start() override;
		void lateUpdate() override;
		void preDestroy() override;
		
		json serialize() override;
		void deserialize(json j) override;

		float density() const;
		void density(float const& value);

		/**
		 * Gets the friction coefficient, usually in the range [0, 1].
		 */
		float friction() const;
		/**
		 * Sets the friction coefficient.
		 */
		void friction(float const& value);

		/**
		 * Gets the restitution (elasticity/bounciness) usually in the range [0, 1].
		 */
		float restitution() const;
		/**
		 * Sets the restitution.
		 */
		void restitution(float const& value);

		/**
		 * Gets if the collider is a sensor or not.
		 *
		 * If the collider is a sensor, it acts as an invisible collider.
		 * It's still able to receive collision (ISensorBegin & ISensorEnd) callbacks but other colliders/bodies will act like the collider doesn't exist.
		 */
		bool sensor() const;
		/**
		 * Sets if the collider is a sensor or not.
		 */
		void sensor(bool const& value);

		/**
		 * Gets the offset of the collider relative to the Actor's position in pixels.
		 */
		Vector2 offset() const;
		/**
		 * Sets the position offset.
		 */
		void offset(Vector2 const& value);

		/**
		 * Gets the rotational offset of the collider relative to the Actor's rotation in degrees.
		 */
		float rotationOffset() const;
		/**
		 * Sets the rotation offset.
		 */
		void rotationOffset(float const& value);
		
	protected:
		Vector2 _offset;
		float _rotationOffset = 0;
		
		//Simulation values
		float _density = 1;
		float _friction = 0;
		float _restitution = 0;

		bool _sensor = false;
		
		Unique<b2Shape> shape;
		b2Body* body = nullptr;
		bool isDirty = false;
		
		/**
		 * Required to set Collider::shape, this function defines the shape of the collider and is
		 * set to be overridden by derived classes such as BoxCollider & CircleCollider.
		 */
		virtual void createShape(bool const& includeBodyTransform) = 0;
		b2Fixture* fixture = nullptr;

		/**
		 * Returns Collider::shape, calls createShape() if deemed necessary.
		 */
		b2Shape* getShape(bool const& includeOwnerTransform = false);

		void addSelf();
		void removeSelf();
		void setPhysicsBody(b2Body* newBody);
	};
}
