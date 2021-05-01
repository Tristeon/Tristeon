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
			//Separate domain
			const auto idx = path.find_first_of(':');
			if (idx == std::string::npos || idx == 1) //1 implies global path (e.g. C:/)
				return path;

			const auto domain = StringHelper::toLower(path.substr(0, idx));

			//Find domain path function
			if (getMap()->find(domain) == getMap()->end())
			{
				Console::log("Domain " + domain + " doesn't exist!");
				return path;
			}

			//Resolve
			const auto domainPath = (*getMap())[domain];
			return domainPath() + "//" + path.substr(idx + 1, String::npos);
		}
	};
}

#define REGISTER_DOMAIN(domain, func) static const Domain register_domain { domain, func };
