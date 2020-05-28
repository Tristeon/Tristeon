#pragma once
#include <map>
#include <string>

#include <TypeDefinitions.h>
#include <QDir>

namespace Tristeon
{
	class AssetDatabase
	{
	public:
		static void add(String const& path);
		static void remove(String const& path);

		static Vector<String> get(String const& extension);
		static String findByName(String const& name);
		static String findByName(String const& name, String const& extension);
		
		static void load();
	private:
		
		static void detectAll();
		static void readDir(QDir const& dir);
		static std::map<String, Vector<String>> assets;
	};
}
