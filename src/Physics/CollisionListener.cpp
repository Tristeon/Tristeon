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
#include <Physics/Callbacks/IPreContact.h>

#include <TObject.h>

namespace Tristeon
{
	//TODO: CollisionListener could use a structural overhaul to reduce code & increase readability
	void CollisionListener::BeginContact(b2Contact* contact)
	{
		b2WorldManifold manifold{};
		contact->GetWorldManifold(&manifold);
		const auto normal = Vector2::convert(manifold.normal);
		
		//Get downcasted types
		auto* a = static_cast<TObject*>(contact->GetFixtureA()->GetUserData());
		auto* b = static_cast<TObject*>(contact->GetFixtureB()->GetUserData());

		auto* aTileLayer = dynamic_cast<TileLayer*>(a);
		auto* bTileLayer = dynamic_cast<TileLayer*>(b);

		auto* aCollider = dynamic_cast<Collider*>(a);
		auto* bCollider = dynamic_cast<Collider*>(b);

		//Tile contact
		if (aTileLayer != nullptr || bTileLayer != nullptr)
		{
			//Collect the right data
			auto* collider = aTileLayer != nullptr ? bCollider : aCollider;
			auto* layer = aTileLayer != nullptr ? aTileLayer : bTileLayer;
			auto* tileFixture = aTileLayer != nullptr ? contact->GetFixtureA() : contact->GetFixtureB();

			Vector2Int tileIndex;
			for (auto const& f : layer->_fixtures)
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
			tileContact.normal = Vector2::convert(manifold.normal);
			
			for (auto* cb : collider->actor()->behaviours<ITileContactBegin>()) cb->tileContactBegin(tileContact);

			auto* cbActor = dynamic_cast<ITileContactBegin*>(collider->actor());
			if (cbActor != nullptr) cbActor->tileContactBegin(tileContact);
		}
		//Normal contact
		else if (aCollider != nullptr && bCollider != nullptr)
		{
			if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor())
			{
				for (auto* cba : aCollider->actor()->behaviours<ISensorBegin>()) cba->sensorBegin({ bCollider, normal });
				for (auto* cbb : bCollider->actor()->behaviours<ISensorBegin>()) cbb->sensorBegin({ aCollider, -normal });

				auto* cbActorA = dynamic_cast<ISensorBegin*>(aCollider->actor());
				if (cbActorA != nullptr) cbActorA->sensorBegin({ bCollider, normal });

				auto* cbActorB = dynamic_cast<ISensorBegin*>(bCollider->actor());
				if (cbActorB != nullptr) cbActorB->sensorBegin({ aCollider, -normal });
			}
			else
			{
				for (auto* cba : aCollider->actor()->behaviours<IContactBegin>()) cba->contactBegin({ bCollider, normal });
				for (auto* cbb : bCollider->actor()->behaviours<IContactBegin>()) cbb->contactBegin({ aCollider, -normal });

				auto* cbActorA = dynamic_cast<IContactBegin*>(aCollider->actor());
				if (cbActorA != nullptr) cbActorA->contactBegin({ bCollider, normal });

				auto* cbActorB = dynamic_cast<IContactBegin*>(bCollider->actor());
				if (cbActorB != nullptr) cbActorB->contactBegin({ aCollider, -normal });
			}
		}
		else
		{
			throw std::exception("This shouldn't happen");
		}
	}

	void CollisionListener::EndContact(b2Contact * contact)
	{
		b2WorldManifold manifold{};
		contact->GetWorldManifold(&manifold);
		const auto normal = Vector2::convert(manifold.normal);
		
		//Get downcasted types
		auto* a = static_cast<TObject*>(contact->GetFixtureA()->GetUserData());
		auto* b = static_cast<TObject*>(contact->GetFixtureB()->GetUserData());

		auto* aTileLayer = dynamic_cast<TileLayer*>(a);
		auto* bTileLayer = dynamic_cast<TileLayer*>(b);

		auto* aCollider = dynamic_cast<Collider*>(a);
		auto* bCollider = dynamic_cast<Collider*>(b);

		//Tile contact
		if (aTileLayer != nullptr || bTileLayer != nullptr)
		{
			//Collect the right data
			auto* collider = aTileLayer != nullptr ? bCollider : aCollider;
			auto* layer = aTileLayer != nullptr ? aTileLayer : bTileLayer;
			auto* tileFixture = aTileLayer != nullptr ? contact->GetFixtureA() : contact->GetFixtureB();

			Vector2Int tileIndex;
			for (auto const& f : layer->_fixtures)
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
			tileContact.normal = Vector2::convert(manifold.normal);
			
			for (auto* cb : collider->actor()->behaviours<ITileContactEnd>()) cb->tileContactEnd(tileContact);

			auto* cbActor = dynamic_cast<ITileContactEnd*>(collider->actor());
			if (cbActor != nullptr) cbActor->tileContactEnd(tileContact);
		}
		//Normal contact
		else if (aCollider != nullptr && bCollider != nullptr)
		{
			if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor())
			{
				for (auto* cba : aCollider->actor()->behaviours<ISensorEnd>()) cba->sensorEnd({ bCollider, normal });
				for (auto* cbb : bCollider->actor()->behaviours<ISensorEnd>()) cbb->sensorEnd({ aCollider, -normal });

				auto* cbActorA = dynamic_cast<ISensorEnd*>(aCollider->actor());
				if (cbActorA != nullptr) cbActorA->sensorEnd({ bCollider, normal });

				auto* cbActorB = dynamic_cast<ISensorEnd*>(bCollider->actor());
				if (cbActorB != nullptr) cbActorB->sensorEnd({ aCollider, -normal });
			}
			else
			{
				for (auto* cba : aCollider->actor()->behaviours<IContactEnd>()) cba->contactEnd({ bCollider, normal });
				for (auto* cbb : bCollider->actor()->behaviours<IContactEnd>()) cbb->contactEnd({ aCollider, -normal });

				auto* cbActorA = dynamic_cast<IContactEnd*>(aCollider->actor());
				if (cbActorA != nullptr) cbActorA->contactEnd({ bCollider, normal });

				auto* cbActorB = dynamic_cast<IContactEnd*>(bCollider->actor());
				if (cbActorB != nullptr) cbActorB->contactEnd({ aCollider, -normal });
			}
		}
	}

	void CollisionListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
		b2WorldManifold manifold{};
		contact->GetWorldManifold(&manifold);
		const auto normal = Vector2::convert(manifold.normal);
		
		//Get downcasted types
		auto* a = static_cast<TObject*>(contact->GetFixtureA()->GetUserData());
		auto* b = static_cast<TObject*>(contact->GetFixtureB()->GetUserData());

		auto* aCollider = dynamic_cast<Collider*>(a);
		auto* bCollider = dynamic_cast<Collider*>(b);

		auto enableContact = true;
		
		if (aCollider != nullptr && bCollider != nullptr)
		{
			for (auto* cba : aCollider->actor()->behaviours<IPreContact>()) {
				if (!cba->preContact({ bCollider, normal }))
					enableContact = false;
			}
			for (auto* cbb : bCollider->actor()->behaviours<IPreContact>()) {
				if (!cbb->preContact({ aCollider, normal }))
					enableContact = false;
			}
			
			auto* cbActorA = dynamic_cast<IPreContact*>(aCollider->actor());
			if (cbActorA != nullptr)
			{
				if (!cbActorA->preContact({ bCollider, normal }))
					enableContact = false;
			}

			auto* cbActorB = dynamic_cast<IPreContact*>(bCollider->actor());
			if (cbActorB != nullptr) {
				if (!cbActorB->preContact({ aCollider, normal }))
					enableContact = false;
			}
		}

		contact->SetEnabled(enableContact);
	}
}