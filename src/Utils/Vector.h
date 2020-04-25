#pragma once
#include <memory>
#include <vector>

namespace Tristeon
{
	/**
	 * Tristeon::Vector is an extension of the standard std::vector. It implements simplified functionality such as contains() and remove()
	 */
	template<class _Ty, class _Alloc = std::allocator<_Ty>>
	class Vector : public std::vector<_Ty, _Alloc>
	{
		typedef std::vector<_Ty, _Alloc> base;
	public:
		explicit Vector(const _Alloc& _Al = _Alloc());
		Vector(std::initializer_list<_Ty> _Ilist, const _Alloc& _Al = _Alloc());

		/**
		* Checks if the given parameter _Ty is contained inside of this vector
		*/
		bool contains(_Ty t);
		/**
		* Removes item t from the vector
		*/
		void remove(_Ty t);
		/**
		 * Removes the item at i from the vector
		 */
		void removeAt(size_t i);
		/**
		 * Adds item t to vector
		 */
		void add(_Ty t);

		_Ty last();
	};

	template <class _Ty, class _Alloc>
	void Vector<_Ty, _Alloc>::remove(_Ty t)
	{
		base::erase(std::remove(base::begin(), base::end(), t), base::end());
	}

	template <class _Ty, class _Alloc>
	void Vector<_Ty, _Alloc>::removeAt(size_t i)
	{
		base::erase(base::begin() + i);
	}

	template <class _Ty, class _Alloc>
	void Vector<_Ty, _Alloc>::add(_Ty t)
	{
		base::push_back(t);
	}

	template <class _Ty, class _Alloc>
	_Ty Vector<_Ty, _Alloc>::last()
	{
		return base::operator[](base::size() - 1);
	}

	template <class _Ty, class _Alloc>
	Vector<_Ty, _Alloc>::Vector(const _Alloc& _Al) : base(_Al) { }

	template <class _Ty, class _Alloc>
	Vector<_Ty, _Alloc>::Vector(std::initializer_list<_Ty> _Ilist, const _Alloc& _Al) : base(_Ilist, _Al) { }

	template <class _Ty, class _Alloc>
	bool Vector<_Ty, _Alloc>::contains(_Ty t)
	{
		return std::find(base::begin(), base::end(), t) != base::end();
	}
}