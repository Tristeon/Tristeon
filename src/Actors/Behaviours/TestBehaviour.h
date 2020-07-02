#pragma once
#include "Behaviour.h"
#include <Serialization/TypeRegister.h>

#include <Callbacks/IUpdate.h>
#include <Callbacks/IStart.h>

#include <Physics/Callbacks/IPreContact.h>

namespace Tristeon
{
	class PhysicsBody;
	
	/**
	 * Temporary test behaviour used to test our behaviour system. Will be removed at release.
	 */
	class TestBehaviour : public Behaviour, public IPreContact, public IUpdate, public IStart
	{
	public:
		void start() override;
		void update() override;

		json serialize() override;
		void deserialize(json j) override;

		bool preContact(Contact const& contact) override;
	protected:
		PhysicsBody* body = nullptr;

		float jumpVelocity = 1500;
		float groundedDistance = 256;
		float runSpeed = 100;
	};

	REGISTER_TYPE(TestBehaviour);
}
