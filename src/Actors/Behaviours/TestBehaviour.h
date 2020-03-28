#pragma once
#include "Behaviour.h"
#include <Serialization/TypeRegister.h>

#include <Callbacks/IUpdate.h>
#include <Callbacks/IStart.h>

namespace Tristeon
{
	class PhysicsBody;
	
	/**
	 * Temporary test behaviour used to test our behaviour system. Will be removed at release.
	 */
	class TestBehaviour : public Behaviour, public IUpdate, public IStart
	{
		REGISTER_TYPE_H(TestBehaviour)
	public:
		void start() override;
		void update() override;

		json serialize() override;
		void deserialize(json j) override;

	protected:
		PhysicsBody* body = nullptr;
	};
}
