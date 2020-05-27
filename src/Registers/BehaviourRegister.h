#pragma once
#include <map>
#include <Actors/Behaviours/Behaviour.h>
#include <Serialization/Typename.h>
#include <Registers/Register.h>

namespace Tristeon
{
	template <typename T>
	struct DerivedBehaviourRegister : Register<Behaviour>
	{
		static_assert(std::is_base_of<Behaviour, T>::value, "T has to be a type of Behaviour!");
		
		DerivedBehaviourRegister()
		{
			getMap()->emplace(TRISTEON_TYPENAME(T), &CreateInstance<T, Behaviour>);
		}
	};

	using BehaviourRegister = Register<Behaviour>;

#ifdef TRISTEON_EDITOR
	/**
	 * Registers the given type into the BehaviourRegister for editor recognition & dynamic creation
	 */
#define REGISTER_BEHAVIOUR_H(t) static Tristeon::DerivedBehaviourRegister<t> behaviour_reg;
	 /**
	  * Registers the given type into the BehaviourRegister for editor recognition & dynamic creation
	  */
#define REGISTER_BEHAVIOUR_CPP(t) Tristeon::DerivedBehaviourRegister<t> t::behaviour_reg;
#else //behaviour registers are only useful in the editor
#define REGISTER_BEHAVIOUR_H(t)
#define REGISTER_BEHAVIOUR_CPP(t)
#endif
}