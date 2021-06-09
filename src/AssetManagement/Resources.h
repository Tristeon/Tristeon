#pragma once
#include <map>
#include <Serialization/Serializable.h>
#include <Serialization/JsonSerializer.h>

#include <Settings.h>
#include <filesystem>

#include <AssetManagement/MetaFiles/MetaFile.h>
#include <AssetManagement/Domains/Domain.h>
#include <AssetManagement/AssetDatabase.h>

namespace Tristeon
{
	class Resources
	{
	public:
		template<typename T>
		static T* jsonLoad(const String& path)
		{
			if (path.empty())
				return nullptr;

			auto globalPath = Domain::resolve(path);

			if (!Domain::isResolved(globalPath) || !std::filesystem::exists(globalPath)) //Couldn't find global path
				globalPath = Domain::resolve("Assets://" + path); //Use assets path by default
			
			if (!std::filesystem::exists(globalPath))
				return nullptr;

			if (_loadedResources.find(globalPath) != _loadedResources.end())
				return (T*)_loadedResources[globalPath].get();

			auto resource = JsonSerializer::deserialize<T>(globalPath);
			_loadedResources[globalPath] = std::move(resource);
			return (T*)_loadedResources[globalPath].get();
		}

		template<typename T>
		static T* load(const String& path)
		{
			//Find complete filepath (path only contains a path relative to the asset folder)
			if (path.empty())
			{
				TRISTEON_WARNING("The given asset/resource path is empty");
				return nullptr;
			}

			auto globalPath = Domain::resolve(path);

			if (!Domain::isResolved(globalPath) || !std::filesystem::exists(globalPath)) //Couldn't find global path
				globalPath = Domain::resolve("Assets://" + path); //Use assets path by default

			if (!std::filesystem::exists(globalPath))
			{
				TRISTEON_WARNING("Resolved path doesn't exist " + globalPath + ". Original path: " + path);
				return nullptr;
			}

			//Load metafile and its associated resource
			if (!std::filesystem::exists(globalPath + ".meta"))
			{
				TRISTEON_WARNING("Meta file for " + globalPath + " doesn't exist. Asset loading can't happen without meta files.");
				return nullptr;
			}
			
			auto metaFile = JsonSerializer::deserialize<MetaFile>(globalPath + ".meta");
			if (!metaFile)
			{
				TRISTEON_WARNING("Couldn't load meta file for " + globalPath + ". The metafile might not exist or might not be registered properly");
				return nullptr;
			}
			
			if (_newLoadedResources.find(metaFile->GUID) != _newLoadedResources.end())
				return (T*)_newLoadedResources[metaFile->GUID].get();

			auto loadedAsset = metaFile->load(globalPath);

			_newLoadedResources[metaFile->GUID] = std::move(loadedAsset);
			return (T*)_newLoadedResources[metaFile->GUID].get();
		}

		template<typename T>
		static T* load(const uint32_t& guid)
		{
			if (_newLoadedResources.find(guid) != _newLoadedResources.end())
				return (T*)_newLoadedResources[guid].get();

			const String path = AssetDatabase::get(guid);
			if (path == "")
			{
				TRISTEON_WARNING("Couldn't load path for guid " + guid);
				return nullptr;
			}

			return load<T>(path);
		}

		static bool loaded(const uint32_t& guid)
		{
			return _newLoadedResources.find(guid) != _newLoadedResources.end();
		}

		static bool loaded(const String& path)
		{
			//Find complete filepath (path only contains a path relative to the asset folder)
			if (path.empty())
			{
				TRISTEON_WARNING("The given asset/resource path is empty");
				return false;
			}

			auto globalPath = Domain::resolve(path);

			if (!Domain::isResolved(globalPath) || !std::filesystem::exists(globalPath)) //Couldn't find global path
				globalPath = Domain::resolve("Assets://" + path); //Use assets path by default

			if (!std::filesystem::exists(globalPath))
			{
				TRISTEON_WARNING("Resolved path doesn't exist " + globalPath + ". Original path: " + path);
				return false;
			}

			//Load metafile and its associated resource
			if (!std::filesystem::exists(globalPath + ".meta"))
			{
				TRISTEON_WARNING("Meta file for " + globalPath + " doesn't exist. Asset loading can't happen without meta files.");
				return false;
			}

			const auto metaFile = JsonSerializer::deserialize<MetaFile>(globalPath + ".meta");
			if (!metaFile)
			{
				TRISTEON_WARNING("Couldn't load meta file for " + globalPath + ". The metafile might not exist or might not be registered properly");
				return false;
			}

			return _newLoadedResources.find(metaFile->GUID) != _newLoadedResources.end();
		}

	private:
		//TODO: Switch to new meta file system
		static std::map<String, Unique<TObject>> _loadedResources;

		static std::map<uint32_t,Unique<TObject>> _newLoadedResources;
	};
}
