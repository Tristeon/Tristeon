#include "PhysicsBody.h"

#include <Math/Math.h>
#include <Scenes/Actors/Actor.h>
#include <Physics/Collider.h>
#include <Physics/PhysicsWorld.h>

#include <box2d/b2_fixture.h>

namespace Tristeon
{
	json PhysicsBody::serialize()
	{
		json j = Behaviour::serialize();
		j["typeID"] = Type<PhysicsBody>::fullName();
		j["type"] = _type;
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
		Behaviour::deserialize(j);
		
		_type = j.value("type", Dynamic);
		_gravityScale = j.value("gravityScale", 1.0f);
		_fixedRotation = j.value("fixedRotation", false);
		_linearDamping = j.value("linearDamping", 0.0f);
		_angularDamping = j.value("angularDamping", 0.01f);
		_enabled = j.value("enabled", true);
		_continuous = j.value("continuous", false);
	}

	void PhysicsBody::start()
	{
		if (body == nullptr)
			createBody();

		for (auto* col : actor()->findBehaviours<Collider>())
		{
			col->setPhysicsBody(body.get());
		}
	}

	void PhysicsBody::fixedUpdate()
	{
		if (!_enabled)
			return;

		const auto p = body->GetPosition();
		actor()->position = PhysicsWorld::metersToPixels({ p.x, p.y });

		if (!_fixedRotation)
			actor()->rotation = -Math::toDegrees(body->GetAngle()); //Box2D's rotation is inversed to Tristeon's.
	}

	void PhysicsBody::preDestroy()
	{
		for (auto* col : actor()->findBehaviours<Collider>())
		{
			col->setPhysicsBody(nullptr);
		}
		
		body.reset();
	}

	void PhysicsBody::BodyDeleter::operator()(b2Body* body)
	{
		b2World* world = PhysicsWorld::instance()->_world.get();
		world->DestroyBody(body);
	}

	void PhysicsBody::applyForce(const Vector& force, const ForceMode& mode)
	{
		Vector const meterForce = PhysicsWorld::pixelsToMeters(force);

		switch (mode)
		{
		case Force:
			body->ApplyForceToCenter(b2Vec2(meterForce.x, meterForce.y), true);
			break;
		case Impulse:
			body->ApplyLinearImpulseToCenter(b2Vec2(meterForce.x, meterForce.y), true);
			break;
		}
	}

	void PhysicsBody::applyForceToPoint(const Vector& force, const Vector& point, const ForceMode& mode)
	{
		auto const meterForce = PhysicsWorld::pixelsToMeters(force);
		auto const meterPoint = PhysicsWorld::pixelsToMeters(point);

		switch (mode)
		{
		case Force:
			body->ApplyForce(b2Vec2(meterForce.x, meterForce.y), b2Vec2(meterPoint.x, meterPoint.y), true);
			break;
		case Impulse:
			body->ApplyLinearImpulse(b2Vec2(meterForce.x, meterForce.y), b2Vec2(meterPoint.x, meterPoint.y), true);
			break;
		}
	}

	void PhysicsBody::applyTorque(const float& torque)
	{
		float const meterTorque = PhysicsWorld::pixelsToMeters(torque);
		body->ApplyTorque(meterTorque, true);
	}

	Vector PhysicsBody::velocity() const
	{
		const auto vel = body->GetLinearVelocity();
		return PhysicsWorld::metersToPixels({ vel.x, vel.y });
	}

	void PhysicsBody::setVelocity(const Vector& value)
	{
		const auto meterValue = PhysicsWorld::pixelsToMeters(value);
		body->SetLinearVelocity({ meterValue.x, meterValue.y });
	}

	void PhysicsBody::setVelocity(const float& x, const float& y)
	{
		setVelocity({ x, y });
	}

	void PhysicsBody::setVelocityX(const float& value)
	{
		setVelocity(value, velocity().y);
	}

	void PhysicsBody::setVelocityY(const float& value)
	{
		setVelocity(velocity().x, value);
	}

	float PhysicsBody::gravityScale() const
	{
		return _gravityScale;
	}

	void PhysicsBody::setGravityScale(const float& value)
	{
		body->SetGravityScale(value);
		_gravityScale = value;
	}

	bool PhysicsBody::fixedRotation() const
	{
		return _fixedRotation;
	}

	void PhysicsBody::setFixedRotation(const bool& value)
	{
		body->SetFixedRotation(value);
		_fixedRotation = value;
	}

	float PhysicsBody::linearDamping() const
	{
		return _linearDamping;
	}

	void PhysicsBody::setLinearDamping(const float& value)
	{
		body->SetLinearDamping(value);
		_linearDamping = value;
	}

	float PhysicsBody::angularDamping() const
	{
		return _angularDamping;
	}

	void PhysicsBody::setAngularDamping(const float& value)
	{
		body->SetAngularDamping(value);
		_angularDamping = value;
	}

	Vector PhysicsBody::position() const
	{
		const auto p = body->GetPosition();
		return PhysicsWorld::metersToPixels({ p.x, p.y });
	}

	void PhysicsBody::setPosition(const Vector& value)
	{
		auto const meterValue = PhysicsWorld::pixelsToMeters(value);
		body->SetTransform(b2Vec2(meterValue.x, meterValue.y), body->GetAngle());
	}

	float PhysicsBody::rotation() const
	{
		return -Math::toDegrees(body->GetAngle());
	}

	void PhysicsBody::setRotation(const float& value)
	{
		float const radianValue = -Math::toRadians(value);
		body->SetTransform(body->GetPosition(), radianValue);
	}

	bool PhysicsBody::continuous() const
	{
		return _continuous;
	}

	void PhysicsBody::setContinuous(const bool& value)
	{
		body->SetBullet(value);
		_continuous = value;
	}

	bool PhysicsBody::enabled() const
	{
		return _enabled;
	}

	void PhysicsBody::setEnabled(const bool& value)
	{
		if (value)
			setPosition(actor()->position);
		
		body->SetEnabled(value);
		_enabled = value;
	}

	void PhysicsBody::setType(const BodyType& type)
	{
		body->SetType(static_cast<b2BodyType>(type));
	}

	PhysicsBody::BodyType PhysicsBody::type() const
	{
		return _type;
	}

	b2Body* PhysicsBody::getBody()
	{
		if (body == nullptr)
			createBody();
		return body.get();
	}

	void PhysicsBody::createBody()
	{
		b2World* world = PhysicsWorld::instance()->_world.get();
		Vector const meterPosition = PhysicsWorld::pixelsToMeters(actor()->position);

		b2BodyDef bodyDef;
		bodyDef.position = b2Vec2(meterPosition.x, meterPosition.y);
		bodyDef.angle = Math::toRadians(-actor()->rotation);
		bodyDef.type = (b2BodyType)_type;
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