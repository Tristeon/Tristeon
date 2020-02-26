#include "Keyboard.h"

namespace Tristeon
{
	std::map<Key, bool> Keyboard::keys;
	std::map<Key, bool> Keyboard::keysPressed;
	std::map<Key, bool> Keyboard::keysReleased;

	bool Keyboard::pressed(Key key)
	{
		if (keysPressed.find(key) == keysPressed.end())
			return false;
		return keysPressed[key];
	}

	bool Keyboard::held(Key key)
	{
		if (keys.find(key) == keys.end())
			return false;
		return keys[key];
	}

	bool Keyboard::released(Key key)
	{
		if (keysReleased.find(key) == keysReleased.end())
			return false;
		return keysReleased[key];
	}

	void Keyboard::onPress(QKeyEvent event)
	{
		keys[(Key)event.key()] = true;
		keysPressed[(Key)event.key()] = true;
		keysReleased[(Key)event.key()] = false;
	}

	void Keyboard::onRelease(QKeyEvent event)
	{
		keys[(Key)event.key()] = false;
		keysPressed[(Key)event.key()] = false;
		keysReleased[(Key)event.key()] = true;
	}

	void Keyboard::reset()
	{
		keysPressed.clear();
		keysReleased.clear();
	}
}
