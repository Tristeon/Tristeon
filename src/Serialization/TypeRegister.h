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

			registerAs<Actor, T>();
			registerAs<Behaviour, T>();
			registerAs<Layer, T>();
		}

	private:
		template<typename Base, typename Q>
		typename std::enable_if<std::is_base_of<Base, Q>::value, void>::type registerAs()
		{
			Register<Base>::getMap()->emplace(TRISTEON_TYPENAME(Q), &CreateInstance<Q, Base>);
		}

		template<typename Base, typename Q>
		typename std::enable_if<!std::is_base_of<Base, Q>::value, void>::type registerAs()
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