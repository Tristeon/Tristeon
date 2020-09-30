#pragma once
#include <map>
#include <Utils/TypeDefinitions.h>
#include <Serialization/Serializable.h>
#include <Serialization/JsonSerializer.h>
#include <Utils/StringHelper.h>

#include "Project.h"
#include <filesystem>

#include "AssetManagement/MetaFiles/MetaFile.h"

namespace Tristeon
{
	class Resources
	{
		//TODO replace path dependency to ID dependency
	public:
		template<typename T>
		static T* jsonLoad(const String& path)
		{
			if (path.empty())
				return nullptr;

			String const globalPath = path.find("Internal/") != String::npos ? path : Project::assetPath() + path; //Add global project path unless if it's internal
			String const lower = StringHelper::toLower(globalPath);

			if (!std::filesystem::exists(lower))
				return nullptr;

			if (_loadedResources.find(lower) != _loadedResources.end())
				return (T*)_loadedResources[lower].get();

			auto resource = JsonSerializer::deserialize<T>(lower);
			_loadedResources[lower] = std::move(resource);
			return (T*)_loadedResources[lower].get();
		}

		///<summary>Expects T to contain a constructor that takes a string without having other parameters</summary>
		template<typename T>
		static T* assetLoad(const String& path)
		{
			if (path.empty())
				return nullptr;

			String const globalPath = path.find("Internal/") != String::npos ? path : Project::assetPath() + path; //Add global project path unless if it's internal
			String const lower = StringHelper::toLower(globalPath);

			if (!std::filesystem::exists(lower))
				return nullptr;

			if (_loadedResources.find(lower) != _loadedResources.end())
				return (T*)_loadedResources[lower].get();

			_loadedResources[lower] = std::make_unique<T>(lower);
			return (T*)_loadedResources[lower].get();
		}

		template<typename T>
		static T* load(const String& path)
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
			
			auto metaFile = JsonSerializer::deserialize<MetaFile>(globalPath + ".meta");
			if (_newLoadedResources.find(metaFile->GUID) != _newLoadedResources.end())
				return (T*)_newLoadedResources[metaFile->GUID].get();

			Unique<TObject> loadedAsset = metaFile->load();

			_newLoadedResources[metaFile->GUID] = std::move(loadedAsset);
			return (T*)_newLoadedResources[metaFile->GUID].get();
		}

		static bool loaded(const String& path)
		{
			String const globalPath = path.find("Internal/") != String::npos ? path : Project::assetPath() + path; //Add global project path unless if it's internal
			String const lower = StringHelper::toLower(globalPath);

			if (lower.empty())
				return false;

			if (_loadedResources.find(lower) != _loadedResources.end())
				return true;

			return false;
		}

	private:
		//TODO: Switch to new meta file system
		static std::map<String, Unique<TObject>> _loadedResources;

		static std::map<int,Unique<TObject>> _newLoadedResources;
	};
}