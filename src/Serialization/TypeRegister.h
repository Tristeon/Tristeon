#pragma once
#include <map>
#include <Registers/Register.h>
#include <Serialization/Typename.h>
#include <Serialization/Serializable.h>

namespace Tristeon
{
	template <typename T>
	struct SerializableRegister : Register<Serializable>
	{
		static_assert(std::is_base_of<Serializable, T>::value, "T has to be a type of Serializable!");

		SerializableRegister()
		{
			getMap()->emplace(TRISTEON_TYPENAME(T), &CreateInstance<T, Serializable>);
		}
	};

	using TypeRegister = Register<Serializable>;

	/**
	 * Registers the given type into the SerializableRegister for object creation through serialization.
	 */
#define REGISTER_TYPE_H(t) static Tristeon::SerializableRegister<t> reg;
	 /**
	  * Registers the given type into the SerializableRegister for object creation through serialization.
	  */
#define REGISTER_TYPE_CPP(t) Tristeon::SerializableRegister<t> t::reg;
}