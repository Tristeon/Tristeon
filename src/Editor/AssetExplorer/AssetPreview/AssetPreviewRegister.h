#pragma once
#include "AssetPreview.h"
#include <map>
#include <typeinfo> //Required by some compilers

#include <Utils/Console.h>
#include <Standard/String.h>
#include <Standard/Unique.h>

namespace TristeonEditor
{
	template <typename T>
	Tristeon::Unique<AssetPreview> createAssetPreview(Tristeon::String pPath) { return std::make_unique<T>(pPath); }

	/**
	 * The AssetPreviewRegister, much like the TypeRegister is a map that is used to create instances of registered types.
	 * The AssetPreviewRegister however, matches extension types to the Editor's types, e.g. an extension "clip" might create an instance of TristeonEditor::AnimationClipPreview
	 * In order to create instances you can call createAssetPreview()
	 */
	struct AssetPreviewRegister
	{
		//Map that contains typename as key and createinstance methods as value
		using EditorMap = std::map<Tristeon::String, Tristeon::Unique<AssetPreview>(*)(Tristeon::String)>;

		/**
		 * Creates instance of an object that inherits from AssetPreview.
		 * The user must take ownership of the instance himself.
		 */
		static Tristeon::Unique<AssetPreview> createInstance(const std::string& pExtension, const Tristeon::String& pItem)
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
	 * The derived register is used to register types into the AssetPreviewRegister's map
	 */
	template <typename T>
	struct DerivedAssetPreviewRegister : AssetPreviewRegister
	{
		static_assert(std::is_base_of<AssetPreview, T>::value, "T has to be a type of AssetPreview!");
		static_assert(std::is_abstract<T>::value == false, "T can not be abstract!");

		explicit DerivedAssetPreviewRegister(Tristeon::String extension)
		{
			getMap()->emplace(extension, &createAssetPreview<T>);
		}
	};

	/**
	 * Binds an asset preview to a given extension
	 */
#define ASSET_PREVIEW(extension, t) static const TristeonEditor::DerivedAssetPreviewRegister<t> asset_preview_reg_##t(extension)
#define ASSET_PREVIEW_NAME(extension, t, name) static const TristeonEditor::DerivedAssetPreviewRegister<t> name(extension)
}