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
			tileContact.tile = layer->tileByIndex(tileIndex);
			tileContact.tileInfo = layer->tileset(tileContact.tile.tileSetID)->tileInfo[tileContact.tile.index];

			for (auto* cb : collider->getOwner()->getBehaviours<ITileContactBegin>()) cb->tileContactBegin(tileContact);

			auto* cbActor = dynamic_cast<ITileContactBegin*>(collider->getOwner());
			if (cbActor != nullptr) cbActor->tileContactBegin(tileContact);
		}
		//Normal contact
		else if (aCollider != nullptr && bCollider != nullptr)
		{
			if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor())
			{
				for (auto* cba : aCollider->getOwner()->getBehaviours<ISensorBegin>()) cba->sensorBegin(bCollider);
				for (auto* cbb : bCollider->getOwner()->getBehaviours<ISensorBegin>()) cbb->sensorBegin(aCollider);

				auto* cbActorA = dynamic_cast<ISensorBegin*>(aCollider->getOwner());
				if (cbActorA != nullptr) cbActorA->sensorBegin(bCollider);

				auto* cbActorB = dynamic_cast<ISensorBegin*>(bCollider->getOwner());
				if (cbActorB != nullptr) cbActorB->sensorBegin(aCollider);
			}
			else
			{
				for (auto* cba : aCollider->getOwner()->getBehaviours<IContactBegin>()) cba->contactBegin(bCollider);
				for (auto* cbb : bCollider->getOwner()->getBehaviours<IContactBegin>()) cbb->contactBegin(aCollider);

				auto* cbActorA = dynamic_cast<IContactBegin*>(aCollider->getOwner());
				if (cbActorA != nullptr) cbActorA->contactBegin(bCollider);

				auto* cbActorB = dynamic_cast<IContactBegin*>(bCollider->getOwner());
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
			tileContact.tile = layer->tileByIndex(tileIndex);
			tileContact.tileInfo = layer->tileset(tileContact.tile.tileSetID)->tileInfo[tileContact.tile.index];

			for (auto* cb : collider->getOwner()->getBehaviours<ITileContactEnd>()) cb->tileContactEnd(tileContact);

			auto* cbActor = dynamic_cast<ITileContactEnd*>(collider->getOwner());
			if (cbActor != nullptr) cbActor->tileContactEnd(tileContact);
		}
		//Normal contact
		else if (aCollider != nullptr && bCollider != nullptr)
		{
			if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor())
			{
				for (auto* cba : aCollider->getOwner()->getBehaviours<ISensorEnd>()) cba->sensorEnd(bCollider);
				for (auto* cbb : bCollider->getOwner()->getBehaviours<ISensorEnd>()) cbb->sensorEnd(aCollider);

				auto* cbActorA = dynamic_cast<ISensorEnd*>(aCollider->getOwner());
				if (cbActorA != nullptr) cbActorA->sensorEnd(bCollider);

				auto* cbActorB = dynamic_cast<ISensorEnd*>(bCollider->getOwner());
				if (cbActorB != nullptr) cbActorB->sensorEnd(aCollider);
			}
			else
			{
				for (auto* cba : aCollider->getOwner()->getBehaviours<IContactEnd>()) cba->contactEnd(bCollider);
				for (auto* cbb : bCollider->getOwner()->getBehaviours<IContactEnd>()) cbb->contactEnd(aCollider);

				auto* cbActorA = dynamic_cast<IContactEnd*>(aCollider->getOwner());
				if (cbActorA != nullptr) cbActorA->contactEnd(bCollider);

				auto* cbActorB = dynamic_cast<IContactEnd*>(bCollider->getOwner());
				if (cbActorB != nullptr) cbActorB->contactEnd(aCollider);
			}
		}
	}
}
