#pragma once
#include <map>
#include <TypeDefinitions.h>
#include <Serialization/Serializable.h>
#include <Serialization/JsonSerializer.h>
#include <Utils/StringHelper.h>
#include "Rendering/Texture.h"

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

			String const lower = StringHelper::toLower(path);

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

			String const lower = StringHelper::toLower(path);

			if (loadedResources.find(lower) != loadedResources.end())
				return (T*)loadedResources[lower].get();

			loadedResources[lower] = std::make_unique<T>(lower);
			return (T*)loadedResources[lower].get();
		}

		static bool loaded(String const& path)
		{
			String const lower = StringHelper::toLower(path);

			if (lower.empty())
				return false;

			if (loadedResources.find(lower) != loadedResources.end())
				return true;

			return false;
		}

	private:
		static std::map<String, Unique<Serializable>> loadedResources;
	};
}
