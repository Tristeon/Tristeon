#pragma once
#include <Collectors/Collector.h>

namespace Tristeon
{
	/**
	 * ICallback is a base class for callback classes and simply adds/removes callback instances to collectors,
	 * enabling highly efficient callbacks, reducing realtime iteration cost.
	 */
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