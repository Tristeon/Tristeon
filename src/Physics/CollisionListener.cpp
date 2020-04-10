#include "CollisionListener.h"

#include <box2d/b2_contact.h>

#include <Actors/Actor.h>
#include <Physics/Collider.h>

#include <Physics/Callbacks/IContactBegin.h>
#include <Physics/Callbacks/IContactEnd.h>
#include <Physics/Callbacks/ISensorBegin.h>
#include <Physics/Callbacks/ISensorEnd.h>

namespace Tristeon
{
	void CollisionListener::BeginContact(b2Contact* contact)
	{
		auto* a = (Collider*)contact->GetFixtureA()->GetUserData();
		auto* b = (Collider*)contact->GetFixtureB()->GetUserData();

		if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor())
		{
			for (auto* cba : a->owner()->behaviours<ISensorBegin>()) cba->sensorBegin(b);
			for (auto* cbb : b->owner()->behaviours<ISensorBegin>()) cbb->sensorBegin(a);

			auto* cbActorA = dynamic_cast<ISensorBegin*>(a->owner());
			if (cbActorA != nullptr) cbActorA->sensorBegin(b);

			auto* cbActorB = dynamic_cast<ISensorBegin*>(b->owner());
			if (cbActorB != nullptr) cbActorB->sensorBegin(a);
		}
		else
		{
			for (auto* cba : a->owner()->behaviours<IContactBegin>()) cba->contactBegin(b);
			for (auto* cbb : b->owner()->behaviours<IContactBegin>()) cbb->contactBegin(a);

			auto* cbActorA = dynamic_cast<IContactBegin*>(a->owner());
			if (cbActorA != nullptr) cbActorA->contactBegin(b);

			auto * cbActorB = dynamic_cast<IContactBegin*>(b->owner());
			if (cbActorB != nullptr) cbActorB->contactBegin(a);
		}
	}

	void CollisionListener::EndContact(b2Contact* contact)
	{
		auto* a = (Collider*)contact->GetFixtureA()->GetUserData();
		auto* b = (Collider*)contact->GetFixtureB()->GetUserData();

		if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor())
		{
			for (auto* cba : a->owner()->behaviours<ISensorEnd>()) cba->sensorEnd(b);
			for (auto* cbb : b->owner()->behaviours<ISensorEnd>()) cbb->sensorEnd(a);

			auto* cbActorA = dynamic_cast<ISensorEnd*>(a->owner());
			if (cbActorA != nullptr) cbActorA->sensorEnd(b);

			auto * cbActorB = dynamic_cast<ISensorEnd*>(b->owner());
			if (cbActorB != nullptr) cbActorB->sensorEnd(a);
		}
		else
		{
			for (auto* cba : a->owner()->behaviours<IContactEnd>()) cba->contactEnd(b);
			for (auto* cbb : b->owner()->behaviours<IContactEnd>()) cbb->contactEnd(a);

			auto* cbActorA = dynamic_cast<IContactEnd*>(a->owner());
			if (cbActorA != nullptr) cbActorA->contactEnd(b);

			auto * cbActorB = dynamic_cast<IContactEnd*>(b->owner());
			if (cbActorB != nullptr) cbActorB->contactEnd(a);
		}
	}
}