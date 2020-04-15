#pragma once
#include "Behaviour.h"
#include <Serialization/TypeRegister.h>

#include <Callbacks/IUpdate.h>
#include <Callbacks/IStart.h>

#include <Physics/Callbacks/IContactBegin.h>
#include <Physics/Callbacks/IContactEnd.h>

#include "Physics/Callbacks/ITileContactBegin.h"
#include "Physics/Callbacks/ITileContactEnd.h"

namespace Tristeon
{
	class PhysicsBody;
	
	/**
	 * Temporary test behaviour used to test our behaviour system. Will be removed at release.
	 */
	class TestBehaviour : public Behaviour, public IUpdate, public IStart, public IContactBegin, public IContactEnd, public ITileContactBegin, public ITileContactEnd
	{
		REGISTER_TYPE_H(TestBehaviour)
	public:
		void start() override;
		void update() override;

		json serialize() override;
		void deserialize(json j) override;

		void contactBegin(Collider* other) override;
		void contactEnd(Collider* other) override;
		
		void tileContactBegin(TileContact const& contact) override;
		void tileContactEnd(TileContact const& contact) override;
	protected:
		PhysicsBody* body = nullptr;
	};
}
