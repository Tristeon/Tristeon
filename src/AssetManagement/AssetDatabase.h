#pragma once
#include <map>
#include <Standard/List.h>
#include <Standard/String.h>

namespace Tristeon
{
	class AssetDatabase
	{
	public:
		static void add(String const& path);
		static void remove(String const& path);

		/**
		 * Gets all assets with the given extension. The extension is expected to be as follows: .extension
		 */
		static List<String> get(String const& extension);
		static String findByName(String const& name);
		static String findByName(String const& name, String const& extension);
		
		static void load();
	private:
		
		static void detectAll();
		static void readDir(String const& dir);
		static std::map<String, List<String>> assets;
	};
}