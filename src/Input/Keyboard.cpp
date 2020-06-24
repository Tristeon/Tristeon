#include "Keyboard.h"

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

	void Keyboard::onPress(QKeyEvent const& event)
	{
		keys[event.key()] = true;
		keysPressed[event.key()] = true;
		keysReleased[event.key()] = false;
	}

	void Keyboard::onRelease(QKeyEvent const& event)
	{
		keys[event.key()] = false;
		keysPressed[event.key()] = false;
		keysReleased[event.key()] = true;
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
