#include "TestBehaviour.h"
#include "Actors/Actor.h"
#include "Input/Keyboard.h"

namespace Tristeon
{
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
