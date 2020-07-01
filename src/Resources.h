#pragma once
#include <map>
#include <TypeDefinitions.h>
#include <Serialization/Serializable.h>
#include <Serialization/JsonSerializer.h>
#include <Utils/StringHelper.h>

#include "Project.h"
#include <filesystem>

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
		static std::map<String, Unique<TObject>> loadedResources;
	};
}
