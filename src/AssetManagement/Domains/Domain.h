#pragma once
#include <map>
#include <Standard/String.h>
#include <Utils/Console.h>
#include <Utils/StringHelper.h>

namespace Tristeon
{
	class Domain
	{
		using TypeMap = std::map<String, String(*)()>;
	
	public:
		Domain(const String& domain, String(*func)())
		{
			getMap()->emplace(StringHelper::toLower(domain), func);
		}

		/**
		 * Returns the full register map.
		 */
		[[nodiscard]] static TypeMap* getMap()
		{
			static TypeMap instance;
			return &instance;
		}
		
		static String resolve(const String& path)
		{
			if (isResolved(path))
				return path;

			const auto idx = path.find_first_of(':');
			const auto domain = StringHelper::toLower(path.substr(0, idx));

			if (idx == 0 || idx == std::string::npos)
				return path;
			
			//Find domain path function
			if (getMap()->find(domain) == getMap()->end())
			{
				Console::log("Domain " + domain + " doesn't exist!");
				return path;
			}

			//Move along path until we find the first normal character again
			//this resolves paths like /////////////////what.png 
			const auto pathIndex = path.find_first_not_of("/\\", idx + 1);

			//Resolve
			const auto domainPath = (*getMap())[domain];
			return domainPath() + "/" + path.substr(pathIndex, String::npos);
		}

		static bool isResolved(const String& path)
		{
			//resolved paths can start with ../ in which case we should return the already localized path
			if (path.size() >= 3 && path[0] == '.' && path[1] == '.' && path[2] == '/')
				return true;

			//can't use executable folder 
			if (path.find_first_of('/') == std::string::npos)
				return false;
			
			const auto idx = path.find_first_of(':');
			if (idx == std::string::npos || idx == 1) //1 implies global path (e.g. C:/)
				return true;

			return false;
		}
	};
}

#define REGISTER_DOMAIN(domain, func) static const Domain register_domain { domain, func };
