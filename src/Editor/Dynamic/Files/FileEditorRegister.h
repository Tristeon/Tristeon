#pragma once
#ifdef TRISTEON_EDITOR
#include "FileEditor.h"

#include <iostream>
#include <map>

#include <TypeDefinitions.h>
#include <Serialization/Typename.h>

namespace TristeonEditor
{
	template <typename T>
	FileEditor* createEditor() { return new T(); }

	/**
	 * The FileEditorRegister, much like the TypeRegister is a map that is used to create instances of registered types.
	 * The FileEditorRegister however, matches extension types to the Editor's types, e.g. an extension "clip" might create an instance of TristeonEditor::AnimationClipEditor
	 * In order to create instances you can call createEditor()
	 */
	struct FileEditorRegister
	{
		//Map that contains typename as key and createinstance methods as value
		using EditorMap = std::map<Tristeon::String, FileEditor*(*)()>;

		/**
		 * Creates instance of an object that inherits from ObjectEditor.
		 * The user must take ownership of the instance himself.
		 */
		static FileEditor* createInstance(const std::string& extension)
		{
			const auto it = getMap()->find(extension);
			if (it == getMap()->end())
			{
				std::cout << "Couldn't find FileEditor for extension " << extension << std::endl;
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
	 * The derived register is used to register types into the FileEditorRegister's map
	 */
	template <typename T>
	struct DerivedFileEditorRegister : FileEditorRegister
	{
		static_assert(std::is_base_of<FileEditor, T>::value, "T has to be a type of FileEditor!");

		explicit DerivedFileEditorRegister(Tristeon::String extension)
		{
			getMap()->emplace(extension, &createEditor<T>);
		}
	};

	/**
	 * Binds a file editor to a given extension
	 */
	#define FILE_EDITOR_H(extension, editor) static DerivedFileEditorRegister<editor> file_editor_reg;
	 /**
	  * Binds a custom editor to a given extension
	  */
	#define FILE_EDITOR_CPP(extension, editor) DerivedFileEditorRegister<editor> editor::file_editor_reg(extension);
}
#endif