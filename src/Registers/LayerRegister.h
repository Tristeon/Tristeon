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
#define REGISTER_LAYER_H(t) static DerivedLayerRegister<t> layer_reg;
	 /**
	  * Registers the given type into the LayerRegister for editor recognition & dynamic creation
	  */
#define REGISTER_LAYER_CPP(t) DerivedLayerRegister<t> t::layer_reg;
#else //layer registers are only useful in the editor
#define REGISTER_LAYER_H(t)
#define REGISTER_LAYER_CPP(t)
#endif
}