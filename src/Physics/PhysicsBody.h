#pragma once
#include <Actors/Behaviours/Behaviour.h>

#include <memory>
#include <box2d/b2_body.h>
#include <Math/Vector2.h>

#include <Serialization/TypeRegister.h>
#include <Callbacks/IStart.h>
#include <Callbacks/IFixedUpdate.h>

namespace Tristeon
{
	class PhysicsBody : public Behaviour, public IStart, public IFixedUpdate
	{
		friend class Collider;
		friend class SceneManager;
		friend class RaycastResult;
		
		REGISTER_TYPE_H(PhysicsBody)
	public:
		json serialize() override;
		void deserialize(json j) override;
		void start() override;
		void fixedUpdate() override;

		struct BodyDeleter
		{
			void operator()(b2Body* body);
		};

		enum Type
		{
			Static = b2BodyType::b2_staticBody,
			Kinematic = b2BodyType::b2_kinematicBody,
			Dynamic = b2BodyType::b2_dynamicBody
		};

		enum ForceMode
		{
			Force,
			Impulse,
		};

		void applyForce(Vector2 const& force, ForceMode const& mode = ForceMode::Force);
		void applyForceToPoint(Vector2 const& force, Vector2 const& point, ForceMode const& mode = ForceMode::Force);
		
		void applyAngularImpulse(float const& impulse);
		void applyTorque(float const& torque);

		Vector2 velocity() const;
		void velocity(Vector2 const& value);

		float gravityScale() const;
		void gravityScale(float const& value);

		bool fixedRotation() const;
		void fixedRotation(bool const& value);

		float linearDamping() const;
		void linearDamping(float const& value);

		float angularDamping() const;
		void angularDamping(float const& value);

		Vector2 position() const;
		void position(Vector2 const& value);

		float rotation() const;
		void rotation(float const& value);

		bool continuous() const;
		void continuous(bool const& value);
		
	protected:
		void createBody();
		void add(Collider* collider);
		void remove(Collider* collider);
		void resetCollider(Collider* collider);

		Type type = Static;

		float _gravityScale = 1;
		bool _fixedRotation = false;

		float _linearDamping = 0.0f;
		float _angularDamping = 0.01f;

		bool _enabled = true;

		bool _continuous = false;

		//TODO: Handle Dirty colliders
		//TODO: Handle removal and addition of colliders
		std::unique_ptr<b2Body, BodyDeleter> body;
		std::map<Collider*, b2Fixture*> fixtures;
	};
}
