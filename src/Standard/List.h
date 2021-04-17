#pragma once
#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <Utils/ClassDefaults.h>
#include "json.h"

using ull = unsigned long long;

namespace Tristeon
{
	/**
	 * A simple and clean implementation of a dynamic array.
	 * This List class mimics a considerable amount of the std::list but provides a cleaner and easier to use interface with more
	 * simplified and readable functionality.
	 */
	template<typename T>
	class List
	{
	public:

		/**
		 * The Iterator used to iterate over the List's members.
		 *
		 * It is used to support range-based for loops & iterator based iteration.
		 * The Iterator class is usually not constructed manually but instead retrieved through the list's member functions begin() and end().
		 */
		class Iterator
		{
		public:
			using iterator_category = std::random_access_iterator_tag;
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
		 * The ConstIterator is the constant counterpart of Iterator. It is used to iterate over the List's members.
		 *
		 * It is used to support range-based for loops & iterator based iteration.
		 * The ConstIterator class is usually not constructed manually but instead retrieved through the list's member functions begin() and end().
		 */
		class ConstIterator
		{
		public:
			using iterator_category = std::random_access_iterator_tag;
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
		 * The default List constructor preemptively creates an empty array of capacity 100. This is similar to calling reserve(100);
		 * This number may be modified by changing the capacity parameter, in which case the array will be of said capacity.
		 * The capacity parameter has to be more than 0, not following this in debug mode will cause a crash but the behaviour is undefined in release mode.
		 */
		explicit List(const ull& capacity = 100);
		/**
		 * Creates a list with an initializer list to support List<T> list = { a, b, c };
		 * The list's capacity will be the same as the list's length.
		 */
		List(const std::initializer_list<T>& list);
		~List();

		List(const List& copy);
		List(List&& move) noexcept;

		List& operator=(const List& copy);
		List& operator=(List&& move) noexcept;

		/**
		 * Empties the list. Leaves the list empty but with a capacity of 1.
		 */
		void clear();

		/**
		 * Returns true if the list is empty. This function doesn't modify the list.
		 */
		[[nodiscard]] bool empty() const;
		/**
		 * Ensures that the list's capacity is at least n.
		 * If n is less than the list's current capacity, nothing happens.
		 */
		void reserve(const ull& n);
		/**
		 * Reduces the list's capacity down to size().
		 */
		ull trim();

		/**
		 * Adds an element to the list.
		 * If the item doesn't fit, the list's capacity is doubled.
		 */
		ull add(T value);
		/**
		 * Adds an element to the list at [index].
		 * Elements at and after [index] are shifted backwards.
		 */
		ull insert(const ull& index, T value);

		/**
		 * Removes (the first occurence of) value from the list.
		 */
		ull remove(T value);
		/**
		 * Removes each occurrence of value from the list.
		 */
		ull removeAll(T value);
		/**
		 * Removes the element at [index] from the list.
		 */
		ull removeAt(const ull& index);
		/**
		 * Removes the last element of the list.
		 * This operation is far more performant than any of the other remove functions,
		 * so if you (for example) wish to iteratively empty a list, going backwards is the best approach.
		 */
		ull removeLast();
		
		/**
		 * The pointer to the list's underlying dynamic array.
		 * This value is altered through the list's member functions and the pointer may turn invalid if the list is modified.
		 */
		[[nodiscard]] T* ptr() const;
		/**
		 * The true size of the internal dynamic array. 
		 */
		[[nodiscard]] ull capacity() const;
		/**
		 * The size of the list.
		 */
		[[nodiscard]] ull size() const;

		[[nodiscard]] Iterator begin() const;
		[[nodiscard]] Iterator end() const;

		/**
		 * The first item of the list.
		 */
		[[nodiscard]] T& first() const;
		/**
		 * The last item of the list.
		 */
		[[nodiscard]] T& last() const;
		
		T& operator[](const ull& index);
		T& operator[](const ull& index) const;

		//TODO: Support indexOf and remove() with a predicate
		/**
		 * Returns true if value is part of the list.
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
		 * Sort the list's elements with the given predicate.
		 */
		template<typename Pred>
		void sort(Pred predicate);
	private:
		T* _ptr = nullptr;
		ull _capacity = 0;
		ull _size = 0;
	};

	template <typename T>
	List<T>::List(const ull& capacity) : _ptr(new T[capacity]), _capacity(capacity), _size(0)
	{
		assert(capacity > 0);
	}

	template <typename T>
	List<T>::List(const std::initializer_list<T>& list) : _ptr(new T[list.size()]), _capacity(list.size()), _size(list.size())
	{
		std::move(list.begin(), list.end(), _ptr);
	}

	template <typename T>
	List<T>::~List()
	{
		delete[] _ptr;
	}

	template <typename T>
	List<T>::List(const List& copy)
	{
		_size = copy._size;
		_capacity = copy._capacity;
		_ptr = new T[copy._capacity];
		std::copy(copy._ptr, copy._ptr + copy._size, _ptr);
	}

	template <typename T>
	List<T>::List(List&& move) noexcept
	{
		_size = move._size;
		_capacity = move._capacity;
		_ptr = move._ptr;

		move._size = 0;
		move._capacity = 0;
		move._ptr = nullptr;
	}

	template <typename T>
	List<T>& List<T>::operator=(const List& copy)
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
	List<T>& List<T>::operator=(List&& move) noexcept
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
	void List<T>::clear()
	{
		delete[] _ptr;
		_ptr = new T[_capacity = 1];
		_size = 0;
	}

	template <typename T>
	bool List<T>::empty() const
	{
		return _size == 0;
	}

	template <typename T>
	ull List<T>::add(T value)
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
	ull List<T>::insert(const ull& index, T value)
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
		//This happens if the index isn't found, so we can assume that it's on an index after oldSize, aka at the end of the list.
		if (_size == oldSize)
			add(std::move(value));

		delete[] old;
		
		return _size;
	}

	template <typename T>
	ull List<T>::remove(T value)
	{
		const ull index = indexOf(value);
		if (index != _size)
			return removeAt(index);
		return _size;
	}

	template <typename T>
	ull List<T>::removeAll(T value)
	{
		ull index = 0;
		while ((index = indexOf(value)) != _size)
			removeAt(index);
		return _size;
	}

	template <typename T>
	ull List<T>::removeAt(const ull& index)
	{
		assert(index < _size);

		//Move everything 1 back
		std::move(_ptr + index + 1, _ptr + _size, _ptr + index);
		return --_size;
	}

	template <typename T>
	ull List<T>::removeLast()
	{
		//Reduce size by 1 and reset the value to default
		operator[](_size - 1) = {};
		return _size--;
	}

	template <typename T>
	void List<T>::reserve(const ull& n)
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
	ull List<T>::trim()
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
	T* List<T>::ptr() const
	{
		return _ptr;
	}

	template <typename T>
	ull List<T>::capacity() const
	{
		return _capacity;
	}

	template <typename T>
	ull List<T>::size() const
	{
		return _size;
	}

	template <typename T>
	typename List<T>::Iterator List<T>::begin() const
	{
		return Iterator(_ptr);
	}

	template <typename T>
	typename List<T>::Iterator List<T>::end() const
	{
		return Iterator(_ptr + _size);
	}

	template <typename T>
	T& List<T>::first() const
	{
		return operator[](0);
	}

	template <typename T>
	T& List<T>::last() const
	{
		return operator[](_size - 1);
	}

	template <typename T>
	T& List<T>::operator[](const ull& index)
	{
		assert(index < _capacity);
		return _ptr[index];
	}

	template <typename T>
	T& List<T>::operator[](const ull& index) const
	{
		assert(index < _size);
		return _ptr[index];
	}

	template <typename T>
	typename List<T>::Iterator begin(const List<T>& list)
	{
		return list.begin();
	}

	template <typename T>
	typename List<T>::Iterator end(const List<T>& list)
	{
		return list.end();
	}

	template <typename T>
	typename List<T>::ConstIterator begin(const List<T>& list)
	{
		return List<T>::ConstIterator(list.ptr());
	}

	template <typename T>
	typename List<T>::ConstIterator end(const List<T>& list)
	{
		return List<T>::ConstIterator(list.ptr() + list.size());
	}

	template <typename T>
	bool List<T>::contains(const T& value) const
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
	bool List<T>::any(pred predicate) const
	{
		for (const auto& element : *this)
		{
			if (predicate(element))
				return true;
		}

		return false;
	}

	template <typename T>
	ull List<T>::indexOf(const T& value)
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
	void List<T>::sort(Pred predicate)
	{
		std::sort(begin(), end(), predicate);
	}

	template<typename T>
	inline void to_json(nlohmann::json& j, const List<T>& p) {
		for (T val : p)
			j.push_back(val);
	}
	template<typename T>
	inline void from_json(const nlohmann::json& j, List<T>& p) {
		for (auto element : j)
			p.add(element.get<T>());
	}
}