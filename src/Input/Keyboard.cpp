#include "Keyboard.h"

#include <iostream>

namespace Tristeon
{
	std::map<int, bool> Keyboard::keys;
	std::map<int, bool> Keyboard::keysPressed;
	std::map<int, bool> Keyboard::keysReleased;

	bool Keyboard::pressed(Key const& key)
	{
		if (keysPressed.find(key) == keysPressed.end())
			return false;
		return keysPressed[key];
	}

	bool Keyboard::held(Key const& key)
	{
		if (keys.find(key) == keys.end())
			return false;
		return keys[key];
	}

	bool Keyboard::released(Key const& key)
	{
		if (keysReleased.find(key) == keysReleased.end())
			return false;
		return keysReleased[key];
	}

	void Keyboard::onPress(Key const& key)
	{
		keys[key] = true;
		keysPressed[key] = true;
		keysReleased[key] = false;
	}

	void Keyboard::onRelease(Key const& key)
	{
		keys[key] = false;
		keysPressed[key] = false;
		keysReleased[key] = true;
	}

	void Keyboard::reset()
	{
		keysPressed.clear();
		keysReleased.clear();
	}

	void Keyboard::clearAll()
	{
		keys.clear();
		keysPressed.clear();
		keysReleased.clear();
	}
}
