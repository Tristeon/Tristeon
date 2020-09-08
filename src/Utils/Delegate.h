#pragma once
#include <unordered_map>
#include <functional>
#include <algorithm> //Required in GNU GCC
#include <Utils/Random.h>

namespace Tristeon
{
	/**
	* Delegate is a callback class that wraps around a vector containing functions.
	* You can add or remove functions from this list, or call invoke() to call all subscribed functions.
	*/
	template <typename ... P>
	class Delegate
	{
	public:
		/**
		 * Calls all the functions added to this delegate.
		 */
		void invoke(P... params);

		/**
		* Calls all the functions added to this delegate.
		*/
		void operator()(std::function<void(P...)> f);

		/**
		 * Adds a function to the delegate. The function must have the same template as the Delegate.
		 *
		 * \returns Returns an ID integer which can be used to remove the callback again (with the -= operator)
		 */
		int operator+=(std::function<void(P...)> f);

		/**
		 * Removes the function at the given ID from the delegate.
		 * If this delegate doesn't contain the given ID this will be ignored.
		 */
		void operator-=(int id);

		/**
		 * Resets the delegate and assigns it to the specific given function.
		 */
		Delegate<P...>& operator=(std::function<void(P...)> f);

		/**
		 * Removes all functions from the delegate.
		 */
		void clear();

	private:
		std::unordered_map<int, std::function<void(P...)>> _events{};
	};

	template <typename ... P>
	void Delegate<P...>::invoke(P... params)
	{
		for (auto& p : _events)
			p.second(params...);
	}

	template <typename ... P>
	void Delegate<P...>::operator()(std::function<void(P...)> f)
	{
		this->invoke(f);
	}

	template <typename ... P>
	int Delegate<P...>::operator+=(std::function<void(P...)> f)
	{
		auto id = Random::generateInt();
		assert(_events.find(id) == _events.end());
		_events[id] = f;
		return id;
	}

	template <typename ... P>
	void Delegate<P...>::operator-=(int id)
	{
		if (_events.find(id) != _events.end())
			_events.erase(id);
	}

	template <typename ... P>
	Delegate<P...>& Delegate<P...>::operator=(std::function<void(P...)> f)
	{
		clear();
		operator+=(f);
		return *this;
	}

	template <typename ... P>
	void Delegate<P...>::clear()
	{
		_events.clear();
	}
}