#include "TestBehaviour.h"
#include "Actors/Actor.h"
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
		if (Keyboard::held(Key_W))
			owner()->position.y += 2;
		if (Keyboard::held(Key_S))
			owner()->position.y -= 2;

		if (Keyboard::held(Key_A))
			owner()->position.x -= 2;
		if (Keyboard::held(Key_D))
			owner()->position.x += 2;
	}
}
