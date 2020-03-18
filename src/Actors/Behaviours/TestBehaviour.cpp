#include "TestBehaviour.h"
#include "Actors/Actor.h"
#include "Input/Gamepad.h"
#include "Input/Keyboard.h"

namespace Tristeon
{
	REGISTER_TYPE_CPP(TestBehaviour)

	json TestBehaviour::serialize()
	{
		json j;
		j["typeID"] = TRISTEON_TYPENAME(TestBehaviour);
		return j;
	}

	void TestBehaviour::deserialize(json j)
	{
		//Empty
	}

	void TestBehaviour::update()
	{
		if (Gamepad::held(Gamepad::X))
			printf("Holding x...");

		owner()->position.x += Gamepad::axisLeft().x;
		owner()->position.y += Gamepad::axisLeft().y;

		owner()->rotation++;
	}
}
