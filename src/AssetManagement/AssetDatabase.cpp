#include "AssetManagement/AssetDatabase.h"
#include <Serialization/JsonSerializer.h>
#include "Project.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace Tristeon
{
	std::map<String, Vector<String>> AssetDatabase::assets;

	void AssetDatabase::add(String const& path)
	{
		auto const p = fs::path(path);
		auto const suffix = p.extension().string();
		
		if (assets.find(suffix) == assets.end())
			assets[suffix] = Vector<String>();

		if (!assets[suffix].contains(path))
			assets[suffix].add(path);
	}

	void AssetDatabase::remove(String const& path)
	{
		auto const p = fs::path(path);
		if (assets.find(p.extension().string()) == assets.end())
			assets[p.extension().string()] = Vector<String>();
		assets[p.extension().string()].remove(path);
	}

	Vector<String> AssetDatabase::get(String const& extension)
	{
		if (assets.find(extension) != assets.end())
			return assets[extension];
		return Vector<String>();
	}

	String AssetDatabase::findByName(String const& name)
	{
		for (const auto& pair : assets)
		{
			for (String path : pair.second)
			{
				if (fs::path(path).stem() == name)
					return path;
			}
		}
		return "";
	}

	String AssetDatabase::findByName(String const& name, String const& extension)
	{
		Vector<String> paths = get(extension);
		for (String path : paths)
		{
			if (fs::path(path).stem() == name)
				return path;
		}

		return "";
	}

	void AssetDatabase::load()
	{
		assets.clear();
		detectAll();
	}

	void AssetDatabase::detectAll()
	{
		readDir(Project::assetPath());
	}

	void AssetDatabase::readDir(std::string const& dir)
	{
		for (auto const& entry : fs::directory_iterator(dir))
		{
			if (entry.is_directory())
			{
				if (entry.path().filename().string() == "." || entry.path().filename().string() == ".." || entry.path().filename().string().find("tmp"))
					continue;
				std::cout << "Path: " << entry.path() << std::endl;
				readDir(entry.path().string());
			}
			else
			{
				if (assets.find(entry.path().extension().string()) == assets.end())
					assets[entry.path().extension().string()] = Vector<String>();

				assets[entry.path().extension().string()].add(relative(entry.path(), std::filesystem::path(Project::assetPath())).string());
			}
		}
	}
}