#include "Resources.h"

namespace Tristeon
{
	std::map<std::string, std::unique_ptr<TObject>> Resources::loadedResources{};

	std::map<int,std::unique_ptr<TObject>> Resources::newLoadedResources{};
}
