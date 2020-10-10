#pragma once
#ifdef TRISTEON_EDITOR
#include "SceneEditor.h"

#include <iostream>
#include <map>

#include <Utils/TypeDefinitions.h>
#include <Serialization/Type.h>

namespace TristeonEditor
{
	template <typename T>
	SceneEditor* createEditor() { return new T(); }

	/**
	 * The SceneEditorRegister, much like the TypeRegister is a map that is used to create instances of registered types.
	 * The SceneEditorRegister however, matches classes' typenames to the Editor's types, e.g. a typename "Tristeon::ActorLayer" might create an instance of TristeonEditor::ActorLayerSceneEditor
	 * In order to create instances you can call createEditor()
	 */
	struct SceneEditorRegister
	{
		//Map that contains typename as key and createinstance methods as value
		using EditorMap = std::map<Tristeon::String, SceneEditor* (*)()>;

		/**
		 * Creates instance of an object that inherits from SceneEditor.
		 * The user must take ownership of the instance himself.
		 */
		static SceneEditor* createInstance(const std::string& s)
		{
			const auto it = getMap()->find(s);
			if (it == getMap()->end())
			{
				std::cout << "Couldn't find SceneEditor for type " << s << std::endl;
				return nullptr;
			}
			return it->second();
		}

		/**
		 * Returns the full register
		 */
		static EditorMap* getMap()
		{
			static EditorMap instance;
			return &instance;
		}
	};

	/**
	 * The derived register is used to register types into the SceneEditorRegister's map
	 */
	template <typename T, typename EditorType>
	struct DerivedSceneEditorRegister : SceneEditorRegister
	{
		static_assert(std::is_base_of<SceneEditor, EditorType>::value, "T has to be a type of SceneEditor!");

		DerivedSceneEditorRegister()
		{
			getMap()->emplace(Tristeon::Type<T>::fullName(), &createEditor<EditorType>);
		}
	};

	/**
	 * Binds a custom editor to a given type.
	 */
#define SCENE_EDITOR_H(type, editor) static DerivedSceneEditorRegister<type, editor> scene_editor_reg;
	 /**
	  * Binds a custom editor to a given type.
	  */
#define SCENE_EDITOR_CPP(type, editor) DerivedSceneEditorRegister<type, editor> editor::scene_editor_reg;
}
#endif