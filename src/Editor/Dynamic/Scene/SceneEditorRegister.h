#pragma once
#include "SceneEditor.h"

#include <iostream>
#include <map>

#include <Standard/Unique.h>
#include <Serialization/Type.h>
#include <Scenes/Layers/Layer.h>

namespace TristeonEditor
{
	template <typename T>
	Tristeon::Unique<SceneEditor> createSceneEditor(Tristeon::Layer* pLayer) { return std::make_unique<T>(pLayer); }

	/**
	 * The SceneEditorRegister, much like the TypeRegister is a map that is used to create instances of registered types.
	 * The SceneEditorRegister however, matches classes' typenames to the Editor's types, e.g. a typename "Tristeon::ActorLayer" might create an instance of TristeonEditor::ActorLayerSceneEditor
	 * In order to create instances you can call createEditor()
	 */
	struct SceneEditorRegister
	{
		//Map that contains typename as key and createinstance methods as value
		using EditorMap = std::map<Tristeon::String, Tristeon::Unique<SceneEditor> (*)(Tristeon::Layer*)>;

		/**
		 * Creates instance of an object that inherits from SceneEditor.
		 * The user must take ownership of the instance himself.
		 */
		static Tristeon::Unique<SceneEditor> createInstance(const std::string& typeID, Tristeon::Layer* pLayer)
		{
			const auto it = getMap()->find(typeID);
			if (it == getMap()->end())
			{
				std::cout << "Couldn't find SceneEditor for type " << typeID << std::endl;
				return nullptr;
			}
			return it->second(pLayer);
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
			getMap()->emplace(Tristeon::Type<T>::fullName(), &createSceneEditor<EditorType>);
		}
	};

#define SCENE_EDITOR(type, editor) static const TristeonEditor::DerivedSceneEditorRegister<type, editor> scene_editor_reg_##editor
}