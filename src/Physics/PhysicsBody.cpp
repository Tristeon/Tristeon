#include "PhysicsBody.h"

#include <Math/Math.h>
#include <Actors/Actor.h>
#include <Physics/Collider.h>
#include <Physics/PhysicsWorld.h>

#include <box2d/b2_fixture.h>

namespace Tristeon
{
	REGISTER_BEHAVIOUR_CPP(PhysicsBody)
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
		if (body == nullptr)
			createBody();

		for (auto& col : getOwner()->getBehaviours<Collider>())
		{
			col->setPhysicsBody(body.get());
		}
	}

	void PhysicsBody::fixedUpdate()
	{
		if (!_enabled)
			return;
		
		getOwner()->position = PhysicsWorld::metersToPixels(Vector2::convert(body->GetPosition()));

		if (!_fixedRotation)
			getOwner()->rotation = -Math::toDegrees(body->GetAngle());
	}

	void PhysicsBody::preDestroy()
	{
		for (auto& col : getOwner()->getBehaviours<Collider>())
		{
			col->setPhysicsBody(nullptr);
		}
		
		body.reset();
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

	void PhysicsBody::setVelocity(Vector2 const& value)
	{
		Vector2 const meterValue = PhysicsWorld::pixelsToMeters(value);
		body->SetLinearVelocity(meterValue.convert<b2Vec2>());
	}

	void PhysicsBody::setVelocity(float const& x, float const& y)
	{
		setVelocity({ x, y });
	}

	float PhysicsBody::gravityScale() const
	{
		return _gravityScale;
	}

	void PhysicsBody::setGravityScale(float const& value)
	{
		body->SetGravityScale(value);
		_gravityScale = value;
	}

	bool PhysicsBody::fixedRotation() const
	{
		return _fixedRotation;
	}

	void PhysicsBody::setFixedRotation(bool const& value)
	{
		body->SetFixedRotation(value);
		_fixedRotation = value;
	}

	float PhysicsBody::linearDamping() const
	{
		return _linearDamping;
	}

	void PhysicsBody::setLinearDamping(float const& value)
	{
		body->SetLinearDamping(value);
		_linearDamping = value;
	}

	float PhysicsBody::angularDamping() const
	{
		return _angularDamping;
	}

	void PhysicsBody::setAngularDamping(float const& value)
	{
		body->SetAngularDamping(value);
		_angularDamping = value;
	}

	Vector2 PhysicsBody::position() const
	{
		return PhysicsWorld::metersToPixels(Vector2::convert(body->GetPosition()));
	}

	void PhysicsBody::setPosition(Vector2 const& value)
	{
		Vector2 const meterValue = PhysicsWorld::pixelsToMeters(value);
		body->SetTransform(meterValue.convert<b2Vec2>(), body->GetAngle());
	}

	float PhysicsBody::rotation() const
	{
		return -Math::toDegrees(body->GetAngle());
	}

	void PhysicsBody::setRotation(float const& value)
	{
		float const radianValue = -Math::toRadians(value);
		body->SetTransform(body->GetPosition(), radianValue);
	}

	bool PhysicsBody::continuous() const
	{
		return _continuous;
	}

	void PhysicsBody::setContinuous(bool const& value)
	{
		body->SetBullet(value);
		_continuous = value;
	}

	bool PhysicsBody::enabled() const
	{
		return _enabled;
	}

	void PhysicsBody::setEnabled(bool const& value)
	{
		if (value)
			setPosition(getOwner()->position);
		
		body->SetEnabled(value);
		_enabled = value;
	}

	b2Body* PhysicsBody::getBody()
	{
		if (body == nullptr)
			createBody();
		return body.get();
	}

	void PhysicsBody::createBody()
	{
		b2World* world = PhysicsWorld::instance()->world.get();
		Vector2 const meterPosition = PhysicsWorld::pixelsToMeters(getOwner()->position);

		b2BodyDef bodyDef;
		bodyDef.position = meterPosition.convert<b2Vec2>();
		bodyDef.angle = Math::toRadians(-getOwner()->rotation);
		bodyDef.type = (b2BodyType)type;
		bodyDef.fixedRotation = _fixedRotation;
		bodyDef.gravityScale = _gravityScale;
		bodyDef.allowSleep = true;
		bodyDef.awake = true;
		bodyDef.enabled = _enabled;
		bodyDef.userData = this;
		bodyDef.bullet = _continuous;

		body = std::unique_ptr<b2Body, BodyDeleter>(world->CreateBody(&bodyDef), {});
	}
}
