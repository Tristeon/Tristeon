#pragma once
#include <map>
#include <Standard/List.h>
#include <Standard/String.h>

namespace Tristeon
{
	class AssetDatabase
	{
	public:
		static void add(const String& path);
		static void remove(const String& path);
		static void remove(const uint32_t& guid);

		static String get(const uint32_t& guid);
		static String find(const String& name);
		static String find(const String& name, const String& extension);

		static List<String> findByExtension(const String& extension);
		
		static void load();
	private:
		static void detectAll();
		static void readDir(const String& dir);

		struct Asset
		{
			String path;
			String name;
			String extension;
		};
		
		static std::map<uint32_t, Asset> _assets;
	};
}
