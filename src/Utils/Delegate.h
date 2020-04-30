#pragma once
#include <unordered_map>
#include <functional>
#include <algorithm> //Required in GNU GCC

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
		void operator=(std::function<void(P...)> f);

		/**
		 * Removes all functions from the delegate.
		 */
		void clear();

	private:
		std::unordered_map<int, std::function<void(P...)>> events;
	};

	template <typename ... P>
	void Delegate<P...>::invoke(P... params)
	{
		for (auto& p : events)
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
		int id = rand(); // TODO: Make more safe by generating a nr that wont be replicated 
		assert(events.find(id) == events.end()); 
		events[id] = f;
		return id;
	}

	template <typename ... P>
	void Delegate<P...>::operator-=(int id)
	{
		if (events.find(id) != events.end())
			events.erase(id);
	}

	template <typename ... P>
	void Delegate<P...>::operator=(std::function<void(P...)> f)
	{
		clear();
		operator+=(f);
	}

	template <typename ... P>
	void Delegate<P...>::clear()
	{
		events.clear();
	}
}