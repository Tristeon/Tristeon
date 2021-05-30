#pragma once
#include <map>
#include <Serialization/Serializable.h>
#include <Serialization/JsonSerializer.h>

#include "Settings.h"
#include <filesystem>

#include <AssetManagement/MetaFiles/MetaFile.h>
#include <AssetManagement/Domains/Domain.h>

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

			auto globalPath = Domain::resolve(path);

			if (!Domain::isResolved(globalPath)) //Couldn't find global path
				globalPath = Domain::resolve("Assets://" + path); //Use assets path by default
			
			if (!std::filesystem::exists(globalPath))
				return nullptr;

			if (_loadedResources.find(globalPath) != _loadedResources.end())
				return (T*)_loadedResources[globalPath].get();

			auto resource = JsonSerializer::deserialize<T>(globalPath);
			_loadedResources[globalPath] = std::move(resource);
			return (T*)_loadedResources[globalPath].get();
		}

		///<summary>Expects T to contain a constructor that takes a string without having other parameters</summary>
		template<typename T>
		static T* assetLoad(const String& path)
		{
			if (path.empty())
				return nullptr;

			auto globalPath = Domain::resolve(path);

			if (globalPath.size() >= 2 && globalPath[1] != ':') //Couldn't find global path
				globalPath = Domain::resolve("Assets://" + path); //Use assets path by default
			
			if (!std::filesystem::exists(globalPath))
				return nullptr;

			if (_loadedResources.find(globalPath) != _loadedResources.end())
				return (T*)_loadedResources[globalPath].get();

			_loadedResources[globalPath] = std::make_unique<T>(globalPath);
			return (T*)_loadedResources[globalPath].get();
		}

		template<typename T>
		static T* load(const String& path)
		{
			//Find complete filepath (path only contains a path relative to the asset folder)
			if (path.empty())
				return nullptr;

			auto globalPath = Domain::resolve(path);

			if (globalPath.size() >= 2 && globalPath[1] != ':') //Couldn't find global path
				globalPath = Domain::resolve("Assets://" + path); //Use assets path by default

			if (!std::filesystem::exists(globalPath))
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
			auto globalPath = Domain::resolve(path);

			if (globalPath.size() >= 2 && globalPath[1] != ':') //Couldn't find global path
				globalPath = Domain::resolve("Assets://" + path); //Use assets path by default

			if (globalPath.empty())
				return false;

			if (_loadedResources.find(globalPath) != _loadedResources.end())
				return true;

			return false;
		}

	private:
		//TODO: Switch to new meta file system
		static std::map<String, Unique<TObject>> _loadedResources;

		static std::map<uint32_t,Unique<TObject>> _newLoadedResources;
	};
}
