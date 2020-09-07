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
	 *
	 * Collider uses the Box2D API and as such all coordinates and sizes are internally converted to meters.
	 */
	class Collider : public Behaviour, public IStart, public ILateUpdate, public IPreDestroy
	{
		friend class PhysicsBody;
		friend class PhysicsWorld;
	public:
		void start() override;
		void lateUpdate() override;
		void preDestroy() override;
		
		json serialize() override;
		void deserialize(json j) override;

		/**
		 * The density of the collider's physical material.
		 * This is used together with the surface area to determine the object's mass.
		 */
		[[nodiscard]] float density() const;
		/**
		 * Sets the collider's density.
		 */
		void setDensity(const float& value);

		/**
		 * Gets the friction coefficient, usually in the range [0, 1].
		 */
		[[nodiscard]] float friction() const;
		/**
		 * Sets the friction coefficient.
		 */
		void setFriction(const float& value);

		/**
		 * Gets the restitution (elasticity/bounciness) usually in the range [0, 1].
		 */
		[[nodiscard]] float restitution() const;
		/**
		 * Sets the restitution.
		 */
		void setRestitution(const float& value);

		/**
		 * Gets if the collider is a sensor or not.
		 *
		 * If the collider is a sensor, it acts as an invisible collider.
		 * It's still able to receive collision (ISensorBegin & ISensorEnd) callbacks but other colliders/bodies will act like the collider doesn't exist.
		 */
		[[nodiscard]] bool sensor() const;
		/**
		 * Sets if the collider is a sensor or not.
		 */
		void setSensor(const bool& value);

		/**
		 * Gets the offset of the collider relative to the Actor's position in pixels.
		 */
		[[nodiscard]] Vector2 offset() const;
		/**
		 * Sets the position offset.
		 */
		void setOffset(const Vector2& value);

		/**
		 * Gets the rotational offset of the collider relative to the Actor's rotation in degrees.
		 */
		[[nodiscard]] float rotationOffset() const;
		/**
		 * Sets the rotation offset.
		 */
		void setRotationOffset(const float& value);
		
	protected:
		Vector2 _lastScale;
		Vector2 _offset;
		float _rotationOffset = 0;
		
		//Simulation values
		float _density = 1;
		float _friction = 0;
		float _restitution = 0;

		bool _sensor = false;
		
		Unique<b2Shape> _shape;
		b2Body* _body = nullptr;
		bool _isDirty = false;
		
		/**
		 * Required to set Collider::shape, this function defines the shape of the collider and is
		 * set to be overridden by derived classes such as BoxCollider & CircleCollider.
		 */
		virtual void createShape(const bool& includeBodyTransform) = 0;
		b2Fixture* _fixture = nullptr;

		/**
		 * Returns Collider::shape, calls createShape() if deemed necessary.
		 */
		b2Shape* getShape(const bool& includeOwnerTransform = false);

		void addSelf();
		void removeSelf();
		void setPhysicsBody(b2Body* newBody);
	};
}