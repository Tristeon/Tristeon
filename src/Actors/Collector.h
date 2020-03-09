#pragma once
#include <algorithm>
#include <TypeDefinitions.h>

#include <Actors/Actor.h>
#include <Actors/Behaviours/Behaviour.h>

namespace Tristeon
{
	template<typename T, typename BaseType>
	using InheritsFromBase = std::enable_if_t<std::is_base_of<BaseType, T>::value, T>;
	
	template<typename T, typename BaseType>
	class Collector
	{
		typedef InheritsFromBase<T, BaseType> type;
	public:
		static void add(T* t);
		static void remove(T* t);
		
		static Vector<T*> all();
	private:
		static Vector<T*> collection;
	};

	template<typename T, typename BaseType>
	Vector<T*> Collector<T, BaseType>::collection = {};

	template <typename T, typename BaseType>
	void Collector<T, BaseType>::add(T* t)
	{
		collection.push_back(t);
	}

	template <typename T, typename BaseType>
	void Collector<T, BaseType>::remove(T* t)
	{
		collection.erase(std::remove(collection.begin(), collection.end(), t), collection.end());
	}

	template <typename T, typename BaseType>
	Vector<T*> Collector<T, BaseType>::all()
	{
		return collection;
	}

	template<typename T>
	using BehaviourCollector = Collector<T, Behaviour>;

	template<typename T>
	using ActorCollector = Collector<T, Actor>;
}

