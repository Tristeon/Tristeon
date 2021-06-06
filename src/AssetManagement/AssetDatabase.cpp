#include "AssetManagement/AssetDatabase.h"
#include <AssetManagement/MetaFiles/MetaFile.h>
#include <Serialization/JsonSerializer.h>
#include "Settings.h"
#include <filesystem>


namespace fs = std::filesystem;

namespace Tristeon
{
	std::map<uint32_t, AssetDatabase::Asset> AssetDatabase::_assets;

	void AssetDatabase::add(String const& path)
	{
		const auto metaFile = JsonSerializer::deserialize<MetaFile>(path + ".meta");
		if (!metaFile)
		{
			TRISTEON_WARNING("Couldn't find metafile for asset " + path + ". Asset wasn't added to the database");
			return;
		}

		auto const p = fs::path(path);
		auto const suffix = p.extension().string();
		auto const name = p.stem().string();

		_assets[metaFile->GUID] = Asset{ path, name, suffix };
	}

	void AssetDatabase::remove(String const& path)
	{
		auto const p = fs::path(path);
		uint32_t foundGUID = 0;
		for (auto [guid, asset] : _assets)
		{
			if (asset.path == path)
			{
				foundGUID = guid;
				break;
			}
		}

		if (foundGUID != 0)
			_assets.erase(foundGUID);
	}

	void AssetDatabase::remove(const uint32_t& guid)
	{
		_assets.erase(guid);
	}

	String AssetDatabase::get(const uint32_t& guid)
	{
		if (_assets.find(guid) == _assets.end())
		{
			TRISTEON_WARNING("Couldn't locate asset with GUID " + std::to_string(guid));
			return "";
		}
		return _assets[guid].path;
	}

	String AssetDatabase::find(String const& name)
	{
		for (auto [guid, asset] : _assets)
		{
			if (asset.name == name)
				return asset.path;
		}

		return "";
	}

	String AssetDatabase::find(String const& name, String const& extension)
	{
		for (auto [guid, asset] : _assets)
		{
			if (asset.extension == extension && asset.name == name)
				return asset.path;
		}

		return "";
	}

	List<String> AssetDatabase::findByExtension(const String& extension)
	{
		List<String> result;

		for (auto [guid, asset] : _assets)
		{
			if (asset.extension == extension)
				result.add(asset.path);
		}
		
		return result;
	}

	void AssetDatabase::load()
	{
		_assets.clear();
		detectAll();
	}

	void AssetDatabase::detectAll()
	{
		readDir(Settings::assetPath());
	}

	void AssetDatabase::readDir(String const& dir)
	{
		for (auto const& entry : fs::recursive_directory_iterator(dir))
		{
			if (entry.is_directory())
			{
				if (entry.path().filename().string() == "." || entry.path().filename().string() == ".." || entry.path().filename().string().find("tmp"))
					continue;
				readDir(entry.path().string());
			}
			else
			{
				if (entry.path().extension().string() == ".meta")
					continue;

				add("assets://" + relative(entry.path(), std::filesystem::path(Settings::assetPath())).string());
			}
		}
	}
}