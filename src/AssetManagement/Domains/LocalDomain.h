#pragma once
#include <AssetManagement/Domains/Domain.h>

namespace Tristeon
{
	struct LocalDomain
	{
		static String domainPath()
		{
			return "Local/";
		}
	};

	REGISTER_DOMAIN("Local", &LocalDomain::domainPath);
}
