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
		}
		else
		{
			for (auto* cba : a->owner()->behaviours<IContactBegin>()) cba->contactBegin(b);
			for (auto* cbb : b->owner()->behaviours<IContactBegin>()) cbb->contactBegin(a);
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
		}
		else
		{
			for (auto* cba : a->owner()->behaviours<IContactEnd>()) cba->contactEnd(b);
			for (auto* cbb : b->owner()->behaviours<IContactEnd>()) cbb->contactEnd(a);
		}
	}
}