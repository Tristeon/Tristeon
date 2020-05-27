#pragma once
#include <map>
#include <Scenes/Layers/Layer.h>
#include <Serialization/Typename.h>
#include <Registers/Register.h>

namespace Tristeon
{
	template <typename T>
	struct DerivedLayerRegister : Register<Layer>
	{
		static_assert(std::is_base_of<Layer, T>::value, "T has to be a type of Layer!");

		DerivedLayerRegister()
		{
			getMap()->emplace(TRISTEON_TYPENAME(T), &CreateInstance<T, Layer>);
		}
	};

	using LayerRegister = Register<Layer>;

#ifdef TRISTEON_EDITOR
	/**
	 * Registers the given type into the LayerRegister for editor recognition & dynamic creation
	 */
#define REGISTER_LAYER_H(t) static Tristeon::DerivedLayerRegister<t> layer_reg;
	 /**
	  * Registers the given type into the LayerRegister for editor recognition & dynamic creation
	  */
#define REGISTER_LAYER_CPP(t) Tristeon::DerivedLayerRegister<t> t::layer_reg;
#else //layer registers are only useful in the editor
#define REGISTER_LAYER_H(t)
#define REGISTER_LAYER_CPP(t)
#endif
}