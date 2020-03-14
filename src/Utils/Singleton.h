#pragma once
#include <stdexcept>

namespace Tristeon
{
	/**
	 * Singleton is a base class that helps simplify the implementation of a Singleton pattern into other classes.
	 * It sets a reference to the single instance of the class type and allows access to it through the static instance() function.
	 *
	 * A derived class of this type can only ever have one (1) instance of the passed type.
	 * The constructor of this class will throw an error if a second instance is created.
	 * 
	 * Derived classes ought to pass their own type into the template <T> as such:
	 * class DerivedClass : Singleton<DerivedClass> { };
	 */
	template<typename T>
	class Singleton
	{
	public:
		/**
		 * Returns the current instance of this Singleton.
		 * This instance is not guaranteed to be set and could be nullptr unless if the class is a core subsystem to Tristeon.
		 */
		static T* instance();

		Singleton(Singleton const& other) = delete;
		void operator=(Singleton const& other) = delete;

		Singleton(Singleton&& other) = default;
		Singleton& operator=(Singleton&& other) = default;

	protected:
		/**
		 * Singletons can only be created by derived classes, hence the constructor & destructor are protected.
		 */
		Singleton();
		virtual ~Singleton();
		
	private:
		static T* _instance;
	};

	template<typename T>
	T* Singleton<T>::_instance = nullptr;

	template <typename T>
	Singleton<T>::Singleton()
	{
		if (_instance != nullptr)
			throw std::logic_error("There can only be one instance of a SubSystem type at the same time");
		_instance = static_cast<T*>(this);
	}

	template <typename T>
	Singleton<T>::~Singleton()
	{
		_instance = nullptr;
	}

	template <typename T>
	T* Singleton<T>::instance()
	{
		return _instance;
	}
}
