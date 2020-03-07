#pragma once
#include <iostream>
#include <map>

#include <TypeDefinitions.h>

#include "Typename.h"
#include "Serializable.h"

namespace Tristeon
{
	template <typename T> Unique<Serializable> CreateInstance() { return std::make_unique<T>(); }

	/**
	 * \brief The typeregister pretty much is a map that is used to create instances of registered types
	 * In order to create instances you can call createInstance()
	 */
	struct TypeRegister
	{
		//Map that contains typename as key and createinstance methods as value
		using TypeMap = std::map<String, Unique<Serializable>(*)()>;

		/**
		 * \brief Creates instance of an object that inherits from the introspectioninterface.
		 * The user must take ownership of the instance himself.
		 */
		static Unique<Serializable> createInstance(const std::string& s)
		{
			const auto it = getMap()->find(s);
			if (it == getMap()->end())
			{
				std::cout << "[Warning]: " + s + " is not registered! Use REGISTER_TYPE_H and REGISTER_TYPE_CPP in this class's files!" << std::endl;
				return nullptr;
			}
			return it->second();
		}

		static TypeMap* getMap()
		{
			static TypeMap instance;
			return &instance;
		}
	};

	/**
	 * \brief The derived register is used to register types into the typeregister's map
	 */
	template <typename T>
	struct DerivedRegister : TypeRegister
	{
		DerivedRegister()
		{
			getMap()->emplace(TRISTEON_TYPENAME(T), &CreateInstance<T>);
		}
	};

	/**
	 * \brief Creates a variable called reg using derivedregister, this is needed to register the type into the typeregister
	 */
#define REGISTER_TYPE_H(t) static DerivedRegister<t> reg;
	 /**
	  * \brief Creates the static instance of the derived register which registers the type into the typeregister
	  */
#define REGISTER_TYPE_CPP(t) DerivedRegister<t> t::reg;
}