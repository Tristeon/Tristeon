#pragma once
#include "Behaviour.h"
#include "Callbacks/IUpdate.h"
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	class MouseFollowBehaviour : public Behaviour, public IUpdate
	{
	public:
		[[nodiscard]] json serialize() override;
		void deserialize(json j) override;
		void update() override;
	};

	REGISTER_TYPE(MouseFollowBehaviour);
}