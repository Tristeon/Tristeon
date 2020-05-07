#include "CollisionListener.h"

#include <box2d/b2_contact.h>

#include <Actors/Actor.h>
#include <Physics/Collider.h>
#include <Physics/TileContact.h>
#include <Scenes/Layers/TileLayer.h>

#include <Physics/Callbacks/IContactBegin.h>
#include <Physics/Callbacks/IContactEnd.h>
#include <Physics/Callbacks/ISensorBegin.h>
#include <Physics/Callbacks/ISensorEnd.h>
#include <Physics/Callbacks/ITileContactBegin.h>
#include <Physics/Callbacks/ITileContactEnd.h>

#include <TObject.h>

namespace Tristeon
{
	//TODO: CollisionListener could use a structural overhaul to reduce code & increase readability
	void CollisionListener::BeginContact(b2Contact* contact)
	{
		//Get downcasted types
		TObject* a = static_cast<TObject*>(contact->GetFixtureA()->GetUserData());
		TObject* b = static_cast<TObject*>(contact->GetFixtureB()->GetUserData());

		TileLayer* aTileLayer = dynamic_cast<TileLayer*>(a);
		TileLayer* bTileLayer = dynamic_cast<TileLayer*>(b);

		Collider* aCollider = dynamic_cast<Collider*>(a);
		Collider* bCollider = dynamic_cast<Collider*>(b);

		//Tile contact
		if (aTileLayer != nullptr || bTileLayer != nullptr)
		{
			//Colllect the right data
			Collider* collider = aTileLayer != nullptr ? bCollider : aCollider;
			TileLayer* layer = aTileLayer != nullptr ? aTileLayer : bTileLayer;
			b2Fixture* tileFixture = aTileLayer != nullptr ? contact->GetFixtureA() : contact->GetFixtureB();

			Vector2Int tileIndex;
			for (auto const& f : layer->fixtures)
			{
				if (f.second == tileFixture)
				{
					tileIndex = f.first;
					break;
				}
			}

			//Pass information
			TileContact tileContact;
			tileContact.layer = layer;
			tileContact.tileIndex = tileIndex;
			tileContact.tile = layer->tile(tileIndex);
			tileContact.tileInfo = layer->tileset(tileContact.tile.tileSetID)->tileInfo[tileContact.tile.index];

			for (auto* cb : collider->owner()->behaviours<ITileContactBegin>()) cb->tileContactBegin(tileContact);

			auto* cbActor = dynamic_cast<ITileContactBegin*>(collider->owner());
			if (cbActor != nullptr) cbActor->tileContactBegin(tileContact);
		}
		//Normal contact
		else if (aCollider != nullptr && bCollider != nullptr)
		{
			if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor())
			{
				for (auto* cba : aCollider->owner()->behaviours<ISensorBegin>()) cba->sensorBegin(bCollider);
				for (auto* cbb : bCollider->owner()->behaviours<ISensorBegin>()) cbb->sensorBegin(aCollider);

				auto* cbActorA = dynamic_cast<ISensorBegin*>(aCollider->owner());
				if (cbActorA != nullptr) cbActorA->sensorBegin(bCollider);

				auto* cbActorB = dynamic_cast<ISensorBegin*>(bCollider->owner());
				if (cbActorB != nullptr) cbActorB->sensorBegin(aCollider);
			}
			else
			{
				for (auto* cba : aCollider->owner()->behaviours<IContactBegin>()) cba->contactBegin(bCollider);
				for (auto* cbb : bCollider->owner()->behaviours<IContactBegin>()) cbb->contactBegin(aCollider);

				auto* cbActorA = dynamic_cast<IContactBegin*>(aCollider->owner());
				if (cbActorA != nullptr) cbActorA->contactBegin(bCollider);

				auto* cbActorB = dynamic_cast<IContactBegin*>(bCollider->owner());
				if (cbActorB != nullptr) cbActorB->contactBegin(aCollider);
			}
		}
		else
		{
			throw std::exception("This shouldn't happen");
		}
	}

	void CollisionListener::EndContact(b2Contact * contact)
	{
		//Get downcasted types
		auto* a = (TObject*)contact->GetFixtureA()->GetUserData();
		auto* b = (TObject*)contact->GetFixtureB()->GetUserData();

		auto* aTileLayer = dynamic_cast<TileLayer*>(a);
		auto* bTileLayer = dynamic_cast<TileLayer*>(b);

		auto* aCollider = dynamic_cast<Collider*>(a);
		auto* bCollider = dynamic_cast<Collider*>(b);

		//Tile contact
		if (aTileLayer != nullptr || bTileLayer != nullptr)
		{
			//Colllect the right data
			Collider* collider = aTileLayer != nullptr ? bCollider : aCollider;
			TileLayer* layer = aTileLayer != nullptr ? aTileLayer : bTileLayer;
			b2Fixture* tileFixture = aTileLayer != nullptr ? contact->GetFixtureA() : contact->GetFixtureB();

			Vector2Int tileIndex;
			for (auto const& f : layer->fixtures)
			{
				if (f.second == tileFixture)
				{
					tileIndex = f.first;
					break;
				}
			}

			//Pass information
			TileContact tileContact;
			tileContact.layer = layer;
			tileContact.tileIndex = tileIndex;
			tileContact.tile = layer->tile(tileIndex);
			tileContact.tileInfo = layer->tileset(tileContact.tile.tileSetID)->tileInfo[tileContact.tile.index];

			for (auto* cb : collider->owner()->behaviours<ITileContactEnd>()) cb->tileContactEnd(tileContact);

			auto* cbActor = dynamic_cast<ITileContactEnd*>(collider->owner());
			if (cbActor != nullptr) cbActor->tileContactEnd(tileContact);
		}
		//Normal contact
		else if (aCollider != nullptr && bCollider != nullptr)
		{
			if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor())
			{
				for (auto* cba : aCollider->owner()->behaviours<ISensorEnd>()) cba->sensorEnd(bCollider);
				for (auto* cbb : bCollider->owner()->behaviours<ISensorEnd>()) cbb->sensorEnd(aCollider);

				auto* cbActorA = dynamic_cast<ISensorEnd*>(aCollider->owner());
				if (cbActorA != nullptr) cbActorA->sensorEnd(bCollider);

				auto* cbActorB = dynamic_cast<ISensorEnd*>(bCollider->owner());
				if (cbActorB != nullptr) cbActorB->sensorEnd(aCollider);
			}
			else
			{
				for (auto* cba : aCollider->owner()->behaviours<IContactEnd>()) cba->contactEnd(bCollider);
				for (auto* cbb : bCollider->owner()->behaviours<IContactEnd>()) cbb->contactEnd(aCollider);

				auto* cbActorA = dynamic_cast<IContactEnd*>(aCollider->owner());
				if (cbActorA != nullptr) cbActorA->contactEnd(bCollider);

				auto* cbActorB = dynamic_cast<IContactEnd*>(bCollider->owner());
				if (cbActorB != nullptr) cbActorB->contactEnd(aCollider);
			}
		}
	}
}
