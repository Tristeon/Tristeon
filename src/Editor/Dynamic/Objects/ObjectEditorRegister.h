#pragma once
#ifdef TRISTEON_EDITOR

#include <iostream>
#include <map>

#include <TypeDefinitions.h>
#include <Serialization/Typename.h>
#include <Editor/Dynamic/Objects/ObjectEditor.h>

namespace TristeonEditor
{
	template <typename T>
	ObjectEditor* createEditor() { return new T(); }

	/**
	 * The ObjectEditorRegister, much like the TypeRegister is a map that is used to create instances of registered types.
	 * The ObjectEditorRegister however, matches classes' typenames to the Editor's types, e.g. a typename "Tristeon::ActorLayer" might create an instance of TristeonEditor::ActorLayerEditor
	 * In order to create instances you can call createEditor()
	 */
	struct ObjectEditorRegister
	{
		//Map that contains typename as key and createinstance methods as value
		using EditorMap = std::map<Tristeon::String, ObjectEditor*(*)()>;

		/**
		 * Creates instance of an object that inherits from ObjectEditor.
		 * The user must take ownership of the instance himself.
		 */
		static ObjectEditor* createInstance(const std::string& s)
		{
			const auto it = getMap()->find(s);
			if (it == getMap()->end())
			{
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
	 * The derived register is used to register types into the ObjectEditorRegister's map
	 */
	template <typename Type, typename EditorType>
	struct DerivedObjectEditorRegister : ObjectEditorRegister
	{
		static_assert(std::is_base_of<ObjectEditor, EditorType>::value, "T has to be a type of ObjectEditor!");

		DerivedObjectEditorRegister()
		{
			getMap()->emplace(TRISTEON_TYPENAME(Type), &createEditor<EditorType>);
		}
	};

	/**
	 * Binds a custom editor to a given type.
	 */
	#define OBJECT_EDITOR_H(type, editor) static TristeonEditor::DerivedObjectEditorRegister<type, editor> object_editor_reg;
	 /**
	  * Binds a custom editor to a given type.
	  */
	#define OBJECT_EDITOR_CPP(type, editor) TristeonEditor::DerivedObjectEditorRegister<type, editor> editor::object_editor_reg;
}
#endif