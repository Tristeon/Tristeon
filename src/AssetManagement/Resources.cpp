#include "Resources.h"

namespace Tristeon
{
	std::map<String, std::unique_ptr<TObject>> Resources::_loadedResources{};

	std::map<uint32_t,std::unique_ptr<TObject>> Resources::_newLoadedResources{};
}
