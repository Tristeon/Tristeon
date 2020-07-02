#pragma once
#include "Register.h"

#include <map>
#include <Serialization/Typename.h>
#include <Serialization/Serializable.h>

#include <Actors/Actor.h>
#include <Actors/Behaviours/Behaviour.h>
#include <Scenes/Layers/Layer.h>

namespace Tristeon
{
	template <typename T>
	struct SerializableRegister : Register<Serializable>
	{
		static_assert(std::is_base_of<Serializable, T>::value, "T has to be a type of Serializable!");
		static_assert(!std::is_abstract<T>::value, "Registered types can not be abstract!");
		
		SerializableRegister()
		{
			getMap()->emplace(TRISTEON_TYPENAME(T), &CreateInstance<T, Serializable>);

			registerActor();
			registerBehaviour();
			registerLayer();
		}

	private:
		template<class Q = T>
		typename std::enable_if<std::is_base_of<Actor, Q>::value, void>::type registerActor()
		{
			Register<Actor>::getMap()->emplace(TRISTEON_TYPENAME(Q), &CreateInstance<Q, Actor>);
		}

		template<class Q = T>
		typename std::enable_if<!std::is_base_of<Actor, Q>::value, void>::type registerActor()
		{
			//Empty
		}

		template<class Q = T>
		typename std::enable_if<std::is_base_of<Behaviour, Q>::value, void>::type registerBehaviour()
		{
			Register<Behaviour>::getMap()->emplace(TRISTEON_TYPENAME(Q), &CreateInstance<Q, Behaviour>);
		}

		template<class Q = T>
		typename std::enable_if<!std::is_base_of<Behaviour, Q>::value, void>::type registerBehaviour()
		{
			//Empty
		}

		template<class Q = T>
		typename std::enable_if<std::is_base_of<Layer, Q>::value, void>::type registerLayer()
		{
			Register<Layer>::getMap()->emplace(TRISTEON_TYPENAME(Q), &CreateInstance<Q, Layer>);
		}

		template<class Q = T>
		typename std::enable_if<!std::is_base_of<Layer, Q>::value, void>::type registerLayer()
		{
			//Empty
		}
	};

	using TypeRegister = Register<Serializable>;

/**
 * Registers the given type into the SerializableRegister for object creation through serialization.
 */
#define REGISTER_TYPE(t) static const Tristeon::SerializableRegister<t> register_##t

	//Actor is a unique case of an engine base class that can be instantiated without a derived class.
	//Actor needs to be included in TypeRegister but can't register manually because it'd cause recursive including
	REGISTER_TYPE(Actor);
}