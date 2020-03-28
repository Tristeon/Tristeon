#pragma once
#include <Actors/Collector.h>

namespace Tristeon
{
	template<typename T>
	class ICallback
	{
	protected:
		ICallback();
		virtual ~ICallback();
	};

	template <typename T>
	ICallback<T>::ICallback()
	{
		Collector<T>::add((T*)this);
	}

	template <typename T>
	ICallback<T>::~ICallback()
	{
		Collector<T>::remove((T*)this);
	}
}