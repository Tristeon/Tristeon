#pragma once
#include "AbstractFileEditor.h"
#include <map>
#include <typeinfo> //Required by some compilers

#include <Utils/Console.h>
#include <Standard/String.h>
#include <Standard/Unique.h>

namespace TristeonEditor
{
	template <typename T>
	Tristeon::Unique<AbstractFileEditor> createFileEditor(FileItem pItem) { return std::make_unique<T>(pItem); }

	/**
	 * The FileEditorRegister, much like the TypeRegister is a map that is used to create instances of registered types.
	 * The FileEditorRegister however, matches extension types to the Editor's types, e.g. an extension "clip" might create an instance of TristeonEditor::AnimationClipEditor
	 * In order to create instances you can call createEditor()
	 */
	struct FileEditorRegister
	{
		//Map that contains typename as key and createinstance methods as value
		using EditorMap = std::map<Tristeon::String, Tristeon::Unique<AbstractFileEditor>(*)(FileItem)>;

		/**
		 * Creates instance of an object that inherits from AbstractFileEditor.
		 * The user must take ownership of the instance himself.
		 */
		static Tristeon::Unique<AbstractFileEditor> createInstance(const std::string& pExtension, FileItem pItem)
		{
			const auto it = getMap()->find(pExtension);
			if (it == getMap()->end())
				return nullptr;
			return it->second(pItem);
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
		static_assert(std::is_base_of<AbstractFileEditor, T>::value, "T has to be a type of AbstractFileEditor!");
		static_assert(std::is_abstract<T>::value == false, "T can not be abstract!");

		explicit DerivedFileEditorRegister(Tristeon::String extension)
		{
			getMap()->emplace(extension, &createFileEditor<T>);
		}
	};

	/**
	 * Binds a file editor to a given extension
	 */
	#define FILE_EDITOR(extension, t) static const TristeonEditor::DerivedFileEditorRegister<t> file_editor_reg_##t(extension)
}