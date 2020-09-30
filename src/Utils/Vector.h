#pragma once
#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <Utils/ClassDefaults.h>

using ull = unsigned long long;

namespace Tristeon
{
	/**
	 * A simple and clean implementation of a dynamic array, or as referred to according to C++ standards; a Vector.
	 * This Vector class mimics a considerable amount of the std::vector but provides a cleaner and easier to use interface with more
	 * simplified and readable functionality.
	 */
	template<typename T>
	class Vector
	{
	public:

		/**
		 * The Iterator used to iterate over the Vector's members.
		 *
		 * It is used to support range-based for loops & iterator based iteration.
		 * The Iterator class is usually not constructed manually but instead retrieved through the vector's member functions begin() and end().
		 */
		class Iterator
		{
		public:
			using iterator_category = typename std::random_access_iterator_tag;
			using value_type = T;
			using difference_type = ull;
			using pointer = T*;
			using reference = T&;
			
			explicit Iterator() = default;
			~Iterator() = default;
			
			DEFAULT_MOVE(Iterator);
			DEFAULT_COPY(Iterator);
			
			/**
			 * Constructs an iterator with the given pointer. This pointer is assumed to be a pointer to an array on the heap.
			 * The pointer may be modified through operators like ++ and --.
			 * The pointer's object is non-const and may be modified through the * operator.
			 */
			explicit Iterator(T* ptr) : _ptr(ptr) {}

			bool operator==(const Iterator& rhs) const { return _ptr == rhs._ptr; }
			bool operator!=(const Iterator& rhs) const { return !(*this == rhs); }
			T& operator*() const { return *_ptr; }

			Iterator& operator++() { ++_ptr; return *this; }
			Iterator& operator--() { --_ptr; return *this; }

			difference_type operator-(const Iterator& rhs) const { return _ptr - rhs._ptr; }
			Iterator operator-(const difference_type& off) const { return Iterator(_ptr - off); }
			Iterator operator+(const difference_type& off) const { return Iterator(_ptr + off); }

			bool operator<(const Iterator& rhs) const { return _ptr < rhs._ptr; }
			bool operator>(const Iterator& rhs) const { return _ptr > rhs._ptr; }
		private:
			T* _ptr = nullptr;
		};

		/**
		 * The ConstIterator is the constant counterpart of Iterator. It is used to iterate over the Vector's members.
		 *
		 * It is used to support range-based for loops & iterator based iteration.
		 * The ConstIterator class is usually not constructed manually but instead retrieved through the vector's member functions begin() and end().
		 */
		class ConstIterator
		{
		public:
			using iterator_category = typename std::random_access_iterator_tag;
			using value_type = T;
			using difference_type = ull;
			using pointer = T*;
			using reference = T&;
			
			explicit ConstIterator() = default;
			~ConstIterator() = default;

			DEFAULT_COPY(ConstIterator);
			DEFAULT_MOVE(ConstIterator);
			
			/**
			 * Constructs an iterator with the given pointer. This pointer is assumed to be a pointer to an array on the heap.
			 * The pointer may be modified through operators like ++ and --.
			 * The pointer's object is const and thus can't be modified.
			 */
			explicit ConstIterator(T * ptr) : _ptr(ptr) {}

			bool operator==(const ConstIterator& rhs) const { return _ptr == rhs._ptr; }
			bool operator!=(const ConstIterator& rhs) const { return !(*this == rhs); }
			T& operator*() const { return *_ptr; }

			ConstIterator& operator++() { ++_ptr; return *this; }
			ConstIterator& operator--() { --_ptr; return *this; }

			difference_type operator-(const ConstIterator& rhs) const { return _ptr - rhs._ptr; }
			ConstIterator operator-(const difference_type& diff) const { return ConstIterator(_ptr - diff); }
			ConstIterator operator+(const difference_type& diff) const { return ConstIterator(_ptr + diff); }

			bool operator<(const ConstIterator& rhs) const { return _ptr < rhs._ptr; }
			bool operator>(const ConstIterator& rhs) const { return _ptr > rhs._ptr; }
		private:
			const T* _ptr = nullptr;
		};
		
		/**
		 * The default Vector constructor preemptively creates an empty array of capacity 100. This is similar to calling reserve(100);
		 * This number may be modified by changing the capacity parameter, in which case the array will be of said capacity.
		 * The capacity parameter has to be more than 0, not following this in debug mode will cause a crash but the behaviour is undefined in release mode.
		 *
		 * //TODO: Investigate the possibility of supporting 0 capacity Vectors
		 */
		explicit Vector(const ull& capacity = 100);
		/**
		 * Creates a vector with an initializer list to support Vector<T> vec = { a, b, c };
		 * The vector's capacity will be the same as the list's length.
		 */
		Vector(const std::initializer_list<T>& list);
		~Vector();

		Vector(const Vector& copy);
		Vector(Vector&& move) noexcept;

		Vector& operator=(const Vector& copy);
		Vector& operator=(Vector&& move) noexcept;

		/**
		 * Empties the vector. Leaves the vector empty but with a capacity of 1.
		 */
		void clear();

		/**
		 * Returns true if the vector is empty. This function doesn't modify the vector.
		 */
		[[nodiscard]] bool empty() const;
		/**
		 * Ensures that the vector's capacity is at least n.
		 * If n is less than the vector's current capacity, nothing happens.
		 */
		void reserve(const ull& n);
		/**
		 * Reduces the vector's capacity down to size().
		 */
		ull trim();

		/**
		 * Adds an element to the vector.
		 * If the item doesn't fit, the vector's capacity is doubled.
		 */
		ull add(T value);
		/**
		 * Adds an element to the vector at [index].
		 * Elements at and after [index] are shifted backwards.
		 */
		ull insert(const ull& index, T value);

		/**
		 * Removes (the first occurence of) value from the vector.
		 */
		ull remove(T value);
		/**
		 * Removes each occurrence of value from the vector.
		 */
		ull removeAll(T value);
		/**
		 * Removes the element at [index] from the vector.
		 */
		ull removeAt(const ull& index);
		/**
		 * Removes the last element of the vector.
		 * This operation is far more performant than any of the other remove functions,
		 * so if you (for example) wish to iteratively empty a vector, going backwards is the best approach.
		 */
		ull removeLast();
		
		/**
		 * The pointer to the vector's underlying dynamic array.
		 * This value is altered through the vector's member functions and the pointer may turn invalid if the vector is modified.
		 */
		[[nodiscard]] T* ptr() const;
		/**
		 * The true size of the internal dynamic array. 
		 */
		[[nodiscard]] ull capacity() const;
		/**
		 * The size of the vector.
		 */
		[[nodiscard]] ull size() const;

		[[nodiscard]] Iterator begin() const;
		[[nodiscard]] Iterator end() const;

		/**
		 * The first item of the vector.
		 */
		[[nodiscard]] T& first() const;
		/**
		 * The last item of the vector.
		 */
		[[nodiscard]] T& last() const;
		
		T& operator[](const ull& index);
		T& operator[](const ull& index) const;

		//TODO: Support indexOf and remove() with a predicate
		/**
		 * Returns true if value is part of the vector.
		 */
		[[nodiscard]] bool contains(const T& value) const;

		/**
		 * Iterates over each element and calls predicate.
		 * Returns true if the predicate is true at least once.
		 */
		template <typename pred>
		[[nodiscard]] bool any(pred predicate) const;
		
		/**
		 * Returns the index of the given value.
		 * If the element wasn't found, it returns size().
		 */
		[[nodiscard]] ull indexOf(const T& value);

		/**
		 * Sort the vector's elements with the given predicate.
		 */
		template<typename Pred>
		void sort(Pred predicate);
	private:
		T* _ptr = nullptr;
		ull _capacity = 0;
		ull _size = 0;
	};

	template <typename T>
	Vector<T>::Vector(const ull& capacity) : _ptr(new T[capacity]), _capacity(capacity), _size(0)
	{
		assert(capacity > 0);
	}

	template <typename T>
	Vector<T>::Vector(const std::initializer_list<T>& list) : _ptr(new T[list.size()]), _capacity(list.size()), _size(list.size())
	{
		std::move(list.begin(), list.end(), _ptr);
	}

	template <typename T>
	Vector<T>::~Vector()
	{
		delete[] _ptr;
	}

	template <typename T>
	Vector<T>::Vector(const Vector& copy)
	{
		_size = copy._size;
		_capacity = copy._capacity;
		_ptr = new T[copy._capacity];
		std::copy(copy._ptr, copy._ptr + copy._size, _ptr);
	}

	template <typename T>
	Vector<T>::Vector(Vector&& move) noexcept
	{
		_size = move._size;
		_capacity = move._capacity;
		_ptr = move._ptr;

		move._size = 0;
		move._capacity = 0;
		move._ptr = nullptr;
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(const Vector& copy)
	{
		if (this == &copy)
			return *this;
		
		if (_ptr)
			delete[] _ptr;
		
		_size = copy._size;
		_capacity = copy._capacity;
		_ptr = new T[copy._capacity];
		std::copy(copy._ptr, copy._ptr + copy._size, _ptr);

		return *this;
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(Vector&& move) noexcept
	{
		if (this == &move)
			return *this;
		
		if (_ptr)
			delete[] _ptr;
		
		_size = move._size;
		_capacity = move._capacity;
		_ptr = move._ptr;

		move._size = 0;
		move._capacity = 0;
		move._ptr = nullptr;
		return *this;
	}

	template <typename T>
	void Vector<T>::clear()
	{
		delete[] _ptr;
		_ptr = new T[_capacity = 1];
		_size = 0;
	}

	template <typename T>
	bool Vector<T>::empty() const
	{
		return _size == 0;
	}

	template <typename T>
	ull Vector<T>::add(T value)
	{
		//Increase size if the capacity is too low
		if (_size == _capacity)
		{
			T* old = _ptr;
			_ptr = new T[_capacity = _capacity * 2];
			std::move(old, old + _size, _ptr);
			delete[] old;
		}

		//Assign value and increase size
		_ptr[_size++] = std::move(value);
		return _size;
	}

	template <typename T>
	ull Vector<T>::insert(const ull& index, T value)
	{
		T* old = _ptr;
		const auto oldSize = _size;

		//Recreate, resize if needed
		if (_size == _capacity)
			_capacity *= 2;
		_ptr = new T[_capacity];
		_size = 0;

		//Add each item back iteratively, but sneak value in between once we reach the correct index.
		for (ull i = 0; i < oldSize; i++)
		{
			if (i == index)
				add(std::move(value));
			add(std::move(old[i]));
		}

		//If the size is the same as before then the item wasn't added.
		//This happens if the index isn't found, so we can assume that it's on an index after oldSize, aka at the end of the vector.
		if (_size == oldSize)
			add(std::move(value));

		delete[] old;
		
		return _size;
	}

	template <typename T>
	ull Vector<T>::remove(T value)
	{
		const ull index = indexOf(value);
		if (index != _size)
			return removeAt(index);
		return _size;
	}

	template <typename T>
	ull Vector<T>::removeAll(T value)
	{
		ull index = 0;
		while ((index = indexOf(value)) != _size)
			removeAt(index);
		return _size;
	}

	template <typename T>
	ull Vector<T>::removeAt(const ull& index)
	{
		assert(index < _size);

		//Move everything 1 back
		std::move(_ptr + index + 1, _ptr + _size, _ptr + index);
		return --_size;
	}

	template <typename T>
	ull Vector<T>::removeLast()
	{
		//Reduce size by 1 and reset the value to default
		operator[](_size - 1) = {};
		return _size--;
	}

	template <typename T>
	void Vector<T>::reserve(const ull& n)
	{
		if (n > _capacity)
		{
			T* old = _ptr;
			_ptr = new T[_capacity = n];
			std::copy(old, old + _size, _ptr);
			delete[] old;
		}
	}

	template <typename T>
	ull Vector<T>::trim()
	{
		if (_capacity > _size)
		{
			T* old = _ptr;
			_ptr = new T[_capacity = _size];
			std::move(old, old + _size, _ptr);
			delete[] old;
		}

		return _size;
	}

	template <typename T>
	T* Vector<T>::ptr() const
	{
		return _ptr;
	}

	template <typename T>
	ull Vector<T>::capacity() const
	{
		return _capacity;
	}

	template <typename T>
	ull Vector<T>::size() const
	{
		return _size;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::begin() const
	{
		return Iterator(_ptr);
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::end() const
	{
		return Iterator(_ptr + _size);
	}

	template <typename T>
	T& Vector<T>::first() const
	{
		return operator[](0);
	}

	template <typename T>
	T& Vector<T>::last() const
	{
		return operator[](_size - 1);
	}

	template <typename T>
	T& Vector<T>::operator[](const ull& index)
	{
		assert(index < _capacity);
		return _ptr[index];
	}

	template <typename T>
	T& Vector<T>::operator[](const ull& index) const
	{
		assert(index < _size);
		return _ptr[index];
	}

	template <typename T>
	typename Vector<T>::Iterator begin(const Vector<T>& vec)
	{
		return vec.begin();
	}

	template <typename T>
	typename Vector<T>::Iterator end(const Vector<T>& vec)
	{
		return vec.end();
	}

	template <typename T>
	typename Vector<T>::ConstIterator begin(const Vector<T>& vec)
	{
		return Vector<T>::ConstIterator(vec.ptr());
	}

	template <typename T>
	typename Vector<T>::ConstIterator end(const Vector<T>& vec)
	{
		return Vector<T>::ConstIterator(vec.ptr() + vec.size());
	}

	template <typename T>
	bool Vector<T>::contains(const T& value) const
	{
		for (const auto& element : *this)
		{
			if (element == value)
				return true;
		}
		return false;
	}

	template <typename T>
	template <typename pred>
	bool Vector<T>::any(pred predicate) const
	{
		for (const auto& element : *this)
		{
			if (predicate(element))
				return true;
		}

		return false;
	}

	template <typename T>
	ull Vector<T>::indexOf(const T& value)
	{
		for (ull i = 0; i < _size; i++)
		{
			if (_ptr[i] == value)
				return i;
		}
		return _size;
	}

	template <typename T>
	template <typename Pred>
	void Vector<T>::sort(Pred predicate)
	{
		std::sort(begin(), end(), predicate);
	}
}