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
		return j;
	}

	void PhysicsBody::deserialize(json j)
	{
		type = j["type"];
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

	void PhysicsBody::createBody()
	{
		b2World* world = PhysicsWorld::instance()->world.get();
		Vector2 const meterPosition = PhysicsWorld::pixelsToMeters(owner()->position);

		b2BodyDef bodyDef;
		bodyDef.position = meterPosition.convert<b2Vec2>();
		bodyDef.type = (b2BodyType)type;
		
		body = std::unique_ptr<b2Body, BodyDeleter>(world->CreateBody(&bodyDef), {});
		
		auto colliders = owner()->behaviours<Collider>();

		float collectiveDensity = 0.0f;
		for (auto collider : colliders)
		{
			b2FixtureDef def;
			def.shape = collider->getShape();
			def.density = collider->density();
			def.friction = collider->friction();
			def.isSensor = collider->sensor();

			collectiveDensity += def.density;
			
			b2Fixture* fixture = body->CreateFixture(&def);
			fixtures[collider] = fixture;
		}

		if (collectiveDensity == 0.0f && type == Dynamic)
			std::cout << "A dynamic rigidbody without any density will behave oddly." << std::endl;
			//throw std::logic_error("A dynamic rigidbody must have at least one (1) collider with a density > 0");
	}

	void PhysicsBody::add(Collider* collider)
	{
		if (fixtures.find(collider) == fixtures.end())
			return;

		b2FixtureDef def;
		def.shape = collider->getShape();
		def.density = collider->density();
		def.friction = collider->friction();
		def.isSensor = collider->sensor();
		def.restitution = collider->restitution();
		
		b2Fixture* fixture = body->CreateFixture(&def);
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
	}
}
