#pragma once
#include "Register.h"

#include <map>
#include <Serialization/Typename.h>
#include <Serialization/Serializable.h>

#include <Scenes/Actors/Actor.h>
#include <Scenes/Actors/Behaviour.h>
#include <Scenes/Layers/Layer.h>

namespace Tristeon
{
	/**
	 * SerializableRegister is a specialization of Register.
	 * It's used as the project-wide register for creating instances of Actors, Behaviours and more.
	 *
	 * This class itself shouldn't be interacted with outside of the REGISTER_TYPE macro, because this class is used to register the given type to the Register::map.
	 * Instead, use the alias TypeRegister to create instances with.
	 */
	template <typename T>
	struct SerializableRegister : Register<Serializable>
	{
		static_assert(std::is_base_of<Serializable, T>::value, "T has to be a type of Serializable!");
		static_assert(!std::is_abstract<T>::value, "Registered types can not be abstract!");
		
		SerializableRegister()
		{
			getMap()->emplace(TRISTEON_TYPENAME(T), &CreateInstance<T, Serializable>);

			//Attempt to recognize the type as one of the standard basetypes, add to their respective registers.
			registerAs<Actor, T>();
			registerAs<Behaviour, T>();
			registerAs<Layer, T>();
		}

	private:
		/**
		 * SFINAE approach to recognize T as a given <Base> type. If it's recognized, it's added to the Register<Base> as well.
		 */
		template<typename Base, typename Q>
		typename std::enable_if<std::is_base_of<Base, Q>::value, void>::type registerAs()
		{
			Register<Base>::getMap()->emplace(TRISTEON_TYPENAME(Q), &CreateInstance<Q, Base>);
		}

		/**
		 * SFINAE Failed -> Nothing happens
		 */
		template<typename Base, typename Q>
		typename std::enable_if<!std::is_base_of<Base, Q>::value, void>::type registerAs()
		{
			//Empty
		}
	};

	/**
	 * Use this type alias to create instances of serializables (TypeRegister::createInstance(typename).
	 */
	using TypeRegister = Register<Serializable>;

/**
 * Registers the given type into the SerializableRegister for object creation through serialization.
 */
#define REGISTER_TYPE(t) static const Tristeon::SerializableRegister<t> register_##t

	//Actor is a unique case of an engine base class that can be instantiated without a derived class.
	//Actor needs to be included in TypeRegister but can't register manually because it'd cause recursive including
	REGISTER_TYPE(Actor);
}