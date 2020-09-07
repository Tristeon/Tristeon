#pragma once
#include <TypeDefinitions.h>

namespace Tristeon
{
	/**
	 * Collector is a handy tool for collecting classes of a certain type.
	 *
	 * Classes looking to be collected should add themselves in the constructor using Collector<Type>::add(this) and remove themselves in the deconstructor Collector<Type>::remove(this).
	 * You may then use Collector<Type>::all() to receive all collected objects.
	 *
	 * Default engine classes that are actively tracked:
	 * - Actor
	 * - Behaviour
	 * - Camera
	 * - Shader
	 * 
	 * - IEarlyUpdate
	 * - IUpdate
	 * - IFixedUpdate
	 * - ILateUpdate
	 * - IDrawGizmos
	 */
	template<typename T>
	class Collector
	{
	public:
		/**
		 * Adds an object to the collector.
		 */
		static void add(T* t);

		/**
		 * Removes the object from the collector.
		 */
		static void remove(T* t);

		/**
		 * Returns a vector with all the collected objects.
		 */
		static Vector<T*> all();

	private:
		static Vector<T*> _collection;
	};

	template<typename T>
	Vector<T*> Collector<T>::_collection;

	template <typename T>
	void Collector<T>::add(T* t)
	{
		_collection.add(t);
	}

	template <typename T>
	void Collector<T>::remove(T* t)
	{
		_collection.remove(t);
	}

	template <typename T>
	Vector<T*> Collector<T>::all()
	{
		return _collection;
	}
}