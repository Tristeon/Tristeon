#pragma once
#include <vector>
#include <algorithm>
#include "Behaviour.h"

namespace Tristeon
{
	template<typename T>
	using T_is_behaviour = std::enable_if_t<std::is_base_of<Behaviour, T>::value, T>;
	
	template<typename T>
	class BehaviourCollector
	{
		typedef T_is_behaviour<T> type;
	public:
		static void add(T* t);
		static void remove(T* t);
		
		static std::vector<T*> all();
	private:
		static std::vector<T*> collection;
	};

	template<typename T>
	std::vector<T*> BehaviourCollector<T>::collection = {};

	template <typename T>
	void BehaviourCollector<T>::add(T* t)
	{
		collection.push_back(t);
	}

	template <typename T>
	void BehaviourCollector<T>::remove(T* t)
	{
		collection.erase(std::remove(collection.begin(), collection.end(), t), collection.end());
	}

	template <typename T>
	std::vector<T*> BehaviourCollector<T>::all()
	{
		return collection;
	}
}
