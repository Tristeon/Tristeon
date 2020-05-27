#pragma once
#include <map>
#include <Actors/Actor.h>
#include <Serialization/Typename.h>
#include <Registers/Register.h>

namespace Tristeon
{
	template <typename T>
	struct DerivedActorRegister : Register<Actor>
	{
		static_assert(std::is_base_of<Actor, T>::value, "T has to be a type of Actor!");

		DerivedActorRegister()
		{
			getMap()->emplace(TRISTEON_TYPENAME(T), &CreateInstance<T, Actor>);
		}
	};

	using ActorRegister = Register<Actor>;
	
#ifdef TRISTEON_EDITOR
	/**
	 * Registers the given type into the ActorRegister for editor recognition & dynamic creation
	 */
#define REGISTER_ACTOR_H(t) static Tristeon::DerivedActorRegister<t> actor_reg;
	 /**
	  * Registers the given type into the ActorRegister for editor recognition & dynamic creation
	  */
#define REGISTER_ACTOR_CPP(t) Tristeon::DerivedActorRegister<t> t::actor_reg;
#else //Actor registers are only useful in the editor
#define REGISTER_ACTOR_H(t)
#define REGISTER_ACTOR_CPP(t)
#endif
}
