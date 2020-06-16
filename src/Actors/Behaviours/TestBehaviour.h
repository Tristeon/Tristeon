#pragma once
#include "Behaviour.h"
#include <Serialization/TypeRegister.h>

#include <Callbacks/IUpdate.h>
#include <Callbacks/IStart.h>

#include <Physics/Callbacks/IContactBegin.h>
#include <Physics/Callbacks/IContactEnd.h>

#include <Physics/Callbacks/ITileContactBegin.h>
#include <Physics/Callbacks/ITileContactEnd.h>
#include <Physics/Callbacks/IPreContact.h>

#include <Registers/BehaviourRegister.h>


namespace Tristeon
{
	class PhysicsBody;
	
	/**
	 * Temporary test behaviour used to test our behaviour system. Will be removed at release.
	 */
	class TestBehaviour : public Behaviour, public IPreContact, public IUpdate, public IStart
	{
		REGISTER_BEHAVIOUR_H(TestBehaviour)
		REGISTER_TYPE_H(TestBehaviour)
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
}
