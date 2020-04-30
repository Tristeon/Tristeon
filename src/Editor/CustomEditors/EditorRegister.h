#pragma once
#ifdef TRISTEON_EDITOR

#include <iostream>
#include <map>

#include <TypeDefinitions.h>
#include <Serialization/Typename.h>
#include <Editor/CustomEditors/CustomEditor.h>

namespace TristeonEditor
{
	template <typename T>
	CustomEditor* createEditor() { return new T(); }

	/**
	 * The EditorRegister, much like the TypeRegister is a map that is used to create instances of registered types.
	 * The EditorRegister however, matches classes' typenames to the Editor's types, e.g. a typename "Tristeon::ActorLayer" might create an instance of TristeonEditor::ActorLayerEditor
	 * In order to create instances you can call createEditor()
	 */
	struct EditorRegister
	{
		//Map that contains typename as key and createinstance methods as value
		using EditorMap = std::map<Tristeon::String, CustomEditor*(*)()>;

		/**
		 * Creates instance of an object that inherits from CustomEditor.
		 * The user must take ownership of the instance himself.
		 */
		static CustomEditor* createInstance(const std::string& s)
		{
			const auto it = getMap()->find(s);
			if (it == getMap()->end())
			{
				std::cout << "Couldn't find customeditor for type " << s << std::endl;
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
	 * The derived register is used to register types into the EditorRegister's map
	 */
	template <typename Type, typename EditorType>
	struct DerivedRegister : EditorRegister
	{
		DerivedRegister()
		{
			getMap()->emplace(TRISTEON_TYPENAME(Type), &createEditor<EditorType>);
		}
	};

	/**
	 * Binds a custom editor to a given type.
	 */
	#define CUSTOM_EDITOR_H(type, editor) static DerivedRegister<type, editor> editor_reg;
	 /**
	  * Binds a custom editor to a given type.
	  */
	#define CUSTOM_EDITOR_CPP(type, editor) DerivedRegister<type, editor> editor::editor_reg;
}
#endif