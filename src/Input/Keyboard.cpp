#include "Keyboard.h"

namespace Tristeon
{
	std::map<int, bool> Keyboard::_keys;
	std::map<int, bool> Keyboard::_keysPressed;
	std::map<int, bool> Keyboard::_keysReleased;

	bool Keyboard::pressed(const Key& key)
	{
		if (_keysPressed.find(key) == _keysPressed.end())
			return false;
		return _keysPressed[key];
	}

	bool Keyboard::held(const Key& key)
	{
		if (_keys.find(key) == _keys.end())
			return false;
		return _keys[key];
	}

	bool Keyboard::released(const Key& key)
	{
		if (_keysReleased.find(key) == _keysReleased.end())
			return false;
		return _keysReleased[key];
	}

	void Keyboard::onPress(const Key& key)
	{
		_keys[key] = true;
		_keysPressed[key] = true;
		_keysReleased[key] = false;
	}

	void Keyboard::onRelease(const Key& key)
	{
		_keys[key] = false;
		_keysPressed[key] = false;
		_keysReleased[key] = true;
	}

	void Keyboard::reset()
	{
		_keysPressed.clear();
		_keysReleased.clear();
	}

	void Keyboard::clearAll()
	{
		_keys.clear();
		_keysPressed.clear();
		_keysReleased.clear();
	}
}
