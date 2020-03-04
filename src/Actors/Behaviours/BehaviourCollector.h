#pragma once
#include <vector>
#include <algorithm>
#include "Behaviour.h"
#include <TypeDefinitions.h>

namespace Tristeon
{
	template<typename T>
	using IsBehaviour = std::enable_if_t<std::is_base_of<Behaviour, T>::value, T>;
	
	template<typename T>
	class BehaviourCollector
	{
		typedef IsBehaviour<T> type;
	public:
		static void add(T* t);
		static void remove(T* t);
		
		static Vector<T*> all();
	private:
		static Vector<T*> collection;
	};

	template<typename T>
	Vector<T*> BehaviourCollector<T>::collection = {};

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
	Vector<T*> BehaviourCollector<T>::all()
	{
		return collection;
	}
}
