#pragma once
#include <map>

#include <Standard/String.h>
#include <Standard/Unique.h>

#include <Serialization/Type.h>
#include "AbstractJsonEditor.h"

namespace TristeonEditor
{
	template <typename T>
	Tristeon::Unique<AbstractJsonEditor> createEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback) { return std::make_unique<T>(pValue, pCallback); }
	
	/**
	 * The EditorRegister, much like the TypeRegister is a map that is used to create instances of registered types.
	 * The EditorRegister however, matches classes' typenames to the Editor's types, e.g. a typename "Tristeon::ActorLayer" creates an instance of TristeonEditor::ActorLayerEditor
	 * In order to create instances you can call createInstance()
	 */
	struct EditorRegister
	{
		//Map that contains typename as key and createinstance methods as value
		using EditorMap = std::map<Tristeon::String, Tristeon::Unique<AbstractJsonEditor> (*)(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback)>;

		/**
		 * Creates instance of an object that inherits from AbstractJsonEditor.
		 * The user must take ownership of the instance himself.
		 */
		static Tristeon::Unique<AbstractJsonEditor> createInstance(const std::string& s, const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback)
		{
			const auto it = getMap()->find(s);
			if (it == getMap()->end())
			{
				return nullptr;
			}
			return it->second(pValue, pCallback);
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
	template <typename T, typename EditorType>
	struct DerivedEditorRegister : EditorRegister
	{
		static_assert(std::is_base_of<AbstractJsonEditor, EditorType>::value, "EditorType has to be a type of AbstractJsonEditor!");
		static_assert(std::is_abstract<EditorType>::value == false, "EditorType can't be abstract");

		DerivedEditorRegister()
		{
			getMap()->emplace(Tristeon::Type<T>::fullName(), &createEditor<EditorType>);
		}
	};

#define EDITOR(type, editor) static const TristeonEditor::DerivedEditorRegister<type, editor> editor_reg_##editor
#define EDITOR_(type, editor, name) static const TristeonEditor::DerivedEditorRegister<type, editor> editor_reg_##name
}