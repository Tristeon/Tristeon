#include "PhysicsBody.h"

#include <Math/Math.h>
#include <Actors/Actor.h>
#include <Physics/Collider.h>
#include <Physics/PhysicsWorld.h>

#include <box2d/b2_fixture.h>

namespace Tristeon
{
	REGISTER_TYPE_CPP(PhysicsBody)
	
	json PhysicsBody::serialize()
	{
		json j;
		j["typeID"] = TRISTEON_TYPENAME(PhysicsBody);
		j["type"] = type;
		j["gravityScale"] = _gravityScale;
		j["fixedRotation"] = _fixedRotation;
		j["linearDamping"] = _linearDamping;
		j["angularDamping"] = _angularDamping;
		j["enabled"] = _enabled;
		j["continuous"] = _continuous;
		return j;
	}

	void PhysicsBody::deserialize(json j)
	{
		type = j["type"];
		_gravityScale = j["gravityScale"];
		_fixedRotation = j["fixedRotation"];
		_linearDamping = j["linearDamping"];
		_angularDamping = j["angularDamping"];
		_enabled = j["enabled"];
		_continuous = j["continuous"];
	}

	void PhysicsBody::start()
	{
		createBody();
	}

	void PhysicsBody::fixedUpdate()
	{
		owner()->position = PhysicsWorld::metersToPixels(Vector2::convert(body->GetPosition()));
		owner()->rotation = -Math::toDegrees(body->GetAngle());
	}

	void PhysicsBody::BodyDeleter::operator()(b2Body* body)
	{
		b2World* world = PhysicsWorld::instance()->world.get();
		world->DestroyBody(body);
	}

	void PhysicsBody::applyForce(Vector2 const& force, ForceMode const& mode)
	{
		Vector2 const meterForce = PhysicsWorld::pixelsToMeters(force);

		switch (mode)
		{
		case Force:
			body->ApplyForceToCenter(meterForce.convert<b2Vec2>(), true);
			break;
		case Impulse:
			body->ApplyLinearImpulseToCenter(meterForce.convert<b2Vec2>(), true);
			break;
		}
	}

	void PhysicsBody::applyForceToPoint(Vector2 const& force, Vector2 const& point, ForceMode const& mode)
	{
		Vector2 const meterForce = PhysicsWorld::pixelsToMeters(force);
		Vector2 const meterPoint = PhysicsWorld::pixelsToMeters(point);

		switch (mode)
		{
		case Force:
			body->ApplyForce(meterForce.convert<b2Vec2>(), meterPoint.convert<b2Vec2>(), true);
			break;
		case Impulse:
			body->ApplyLinearImpulse(meterForce.convert<b2Vec2>(), meterPoint.convert<b2Vec2>(), true);
			break;
		}
	}

	void PhysicsBody::applyAngularImpulse(float const& impulse)
	{
		float const meterImpulse = PhysicsWorld::pixelsToMeters(impulse);
		body->ApplyAngularImpulse(meterImpulse, true);
	}
	
	void PhysicsBody::applyTorque(float const& torque)
	{
		float const meterTorque = PhysicsWorld::pixelsToMeters(torque);
		body->ApplyTorque(meterTorque, true);
	}

	Vector2 PhysicsBody::velocity() const
	{
		Vector2 const vel = Vector2::convert(body->GetLinearVelocity());
		return PhysicsWorld::metersToPixels(vel);
	}

	void PhysicsBody::velocity(Vector2 const& value)
	{
		Vector2 const meterValue = PhysicsWorld::pixelsToMeters(value);
		body->SetLinearVelocity(meterValue.convert<b2Vec2>());
	}

	float PhysicsBody::gravityScale() const
	{
		return _gravityScale;
	}

	void PhysicsBody::gravityScale(float const& value)
	{
		body->SetGravityScale(value);
		_gravityScale = value;
	}

	bool PhysicsBody::fixedRotation() const
	{
		return _fixedRotation;
	}

	void PhysicsBody::fixedRotation(bool const& value)
	{
		body->SetFixedRotation(value);
		_fixedRotation = value;
	}

	float PhysicsBody::linearDamping() const
	{
		return _linearDamping;
	}

	void PhysicsBody::linearDamping(float const& value)
	{
		body->SetLinearDamping(value);
		_linearDamping = value;
	}

	float PhysicsBody::angularDamping() const
	{
		return _angularDamping;
	}

	void PhysicsBody::angularDamping(float const& value)
	{
		body->SetAngularDamping(value);
		_angularDamping = value;
	}

	Vector2 PhysicsBody::position() const
	{
		return PhysicsWorld::metersToPixels(Vector2::convert(body->GetPosition()));
	}

	void PhysicsBody::position(Vector2 const& value)
	{
		Vector2 const meterValue = PhysicsWorld::pixelsToMeters(value);
		body->SetTransform(meterValue.convert<b2Vec2>(), body->GetAngle());
	}

	float PhysicsBody::rotation() const
	{
		return -Math::toDegrees(body->GetAngle());
	}

	void PhysicsBody::rotation(float const& value)
	{
		float const radianValue = -Math::toRadians(value);
		body->SetTransform(body->GetPosition(), radianValue);
	}

	bool PhysicsBody::continuous() const
	{
		return _continuous;
	}

	void PhysicsBody::continuous(bool const& value)
	{
		body->SetBullet(value);
		_continuous = value;
	}

	void PhysicsBody::createBody()
	{
		b2World* world = PhysicsWorld::instance()->world.get();
		Vector2 const meterPosition = PhysicsWorld::pixelsToMeters(owner()->position);

		b2BodyDef bodyDef;
		bodyDef.position = meterPosition.convert<b2Vec2>();
		bodyDef.angle = Math::toRadians(-owner()->rotation);
		bodyDef.type = (b2BodyType)type;
		bodyDef.fixedRotation = _fixedRotation;
		bodyDef.gravityScale = _gravityScale;
		bodyDef.allowSleep = true;
		bodyDef.awake = true;
		bodyDef.enabled = _enabled;
		bodyDef.userData = this;
		bodyDef.bullet = _continuous;
		
		body = std::unique_ptr<b2Body, BodyDeleter>(world->CreateBody(&bodyDef), {});
		
		auto colliders = owner()->behaviours<Collider>();

		float collectiveDensity = 0.0f;
		for (auto collider : colliders)
		{
			add(collider);
			collectiveDensity += collider->density();
		}

		if (collectiveDensity == 0.0f && type == Dynamic)
			std::cout << "A dynamic rigidbody without any density will behave oddly." << std::endl;
			//throw std::logic_error("A dynamic rigidbody must have at least one (1) collider with a density > 0");
	}

	void PhysicsBody::add(Collider* collider)
	{
		if (fixtures.find(collider) != fixtures.end())
			return;

		b2FixtureDef def;
		def.shape = collider->getShape();
		def.density = collider->density();
		def.friction = collider->friction();
		def.isSensor = collider->sensor();
		def.restitution = collider->restitution();
		
		b2Fixture* fixture = body->CreateFixture(&def);
		fixture->SetUserData(collider);
		fixtures[collider] = fixture;
	}

	void PhysicsBody::remove(Collider* collider)
	{
		if (fixtures.find(collider) == fixtures.end())
			return;
		body->DestroyFixture(fixtures[collider]);
		fixtures.erase(collider);
	}

	void PhysicsBody::resetCollider(Collider* collider)
	{
		if (fixtures.find(collider) == fixtures.end())
			return;
		
		body->DestroyFixture(fixtures[collider]);
		fixtures.erase(collider);
		add(collider);
		collider->isDirty = false;
	}
}
