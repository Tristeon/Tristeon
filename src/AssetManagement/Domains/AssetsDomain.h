#pragma once
#include <AssetManagement/Domains/Domain.h>

#include "Settings.h"

namespace Tristeon
{
	struct AssetsDomain
	{
		static String domainPath()
		{
			return Settings::assetPath();
		}
	};

	REGISTER_DOMAIN("Assets", &AssetsDomain::domainPath);
}
