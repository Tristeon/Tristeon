#include "Resources.h"

namespace Tristeon
{
	std::map<String, std::unique_ptr<TObject>> Resources::_loadedResources{};

	std::map<int,std::unique_ptr<TObject>> Resources::_newLoadedResources{};
}
