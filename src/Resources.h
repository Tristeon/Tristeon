#pragma once
#include <map>
#include <TypeDefinitions.h>
#include <Serialization/Serializable.h>
#include <Serialization/JsonSerializer.h>
#include <Utils/StringHelper.h>

#include "Project.h"
#include <filesystem>

#include "AssetManagement/MetaFile.h"

namespace Tristeon
{
	class Resources
	{
		//TODO replace path dependency to ID dependency
	public:
		template<typename T>
		static T* jsonLoad(String const& path)
		{
			if (path.empty())
				return nullptr;

			String const globalPath = path.find("Internal/") != String::npos ? path : Project::assetPath() + path; //Add global project path unless if it's internal
			String const lower = StringHelper::toLower(globalPath);

			if (!std::filesystem::exists(lower))
				return nullptr;

			if (loadedResources.find(lower) != loadedResources.end())
				return (T*)loadedResources[lower].get();

			T* resource = JsonSerializer::deserialize<T>(lower);
			loadedResources[lower] = std::move(Unique<T>(resource));
			return (T*)loadedResources[lower].get();
		}

		///<summary>Expects T to contain a constructor that takes a string without having other parameters</summary>
		template<typename T>
		static T* assetLoad(String const& path)
		{
			if (path.empty())
				return nullptr;

			String const globalPath = path.find("Internal/") != String::npos ? path : Project::assetPath() + path; //Add global project path unless if it's internal
			String const lower = StringHelper::toLower(globalPath);

			if (!std::filesystem::exists(lower))
				return nullptr;

			if (loadedResources.find(lower) != loadedResources.end())
				return (T*)loadedResources[lower].get();

			loadedResources[lower] = std::make_unique<T>(lower);
			return (T*)loadedResources[lower].get();
		}

		template<typename T>
		static T* load(String const& path)
		{
			//Find complete filepath (path only contains a path relative to the project)
			if (path.empty())
				return nullptr;

			String const globalPath = path.find("Internal/") != String::npos ? path : Project::assetPath() + path; //Add global project path unless if it's internal
			String const lower = StringHelper::toLower(globalPath);

			if (!std::filesystem::exists(lower))
				return nullptr;

			//Load metafile and its associated resource
			if (!std::filesystem::exists(globalPath + ".meta"))
				return nullptr;
			
			MetaFile* metaFile = JsonSerializer::deserialize<MetaFile>(globalPath + ".meta");
			if (newLoadedResources.find(metaFile->GUID) != newLoadedResources.end())
				return (T*)newLoadedResources[metaFile->GUID].get();

			Unique<TObject> loadedAsset = metaFile->load();

			newLoadedResources[metaFile->GUID] = std::move(loadedAsset);
			return (T*)newLoadedResources[metaFile->GUID].get();
		}

		static bool loaded(String const& path)
		{
			String const globalPath = path.find("Internal/") != String::npos ? path : Project::assetPath() + path; //Add global project path unless if it's internal
			String const lower = StringHelper::toLower(globalPath);

			if (lower.empty())
				return false;

			if (loadedResources.find(lower) != loadedResources.end())
				return true;

			return false;
		}

	private:
		//TODO: Switch to new meta file system
		static std::map<String, Unique<TObject>> loadedResources;

		static std::map<int,Unique<TObject>> newLoadedResources;
	};
}