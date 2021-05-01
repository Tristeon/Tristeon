#pragma once
#include <AssetManagement/Domains/Domain.h>
#include <Settings.h>

namespace Tristeon
{
	struct EngineDomain
	{
		static String domainPath()
		{
			return "Internal/";
		}
	};

	REGISTER_DOMAIN("Engine", &EngineDomain::domainPath);
}
