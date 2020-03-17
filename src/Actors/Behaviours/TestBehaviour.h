#pragma once
#include "Behaviour.h"
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	/**
	 * Temporary test behaviour used to test our behaviour system. Will be removed at release.
	 */
	class TestBehaviour : public Behaviour
	{
		REGISTER_TYPE_H(TestBehaviour)
	public:
		json serialize() override;
		void deserialize(json j) override;

	protected:
		void update() override;
	};
}
