#pragma once
#include <vector>
#include <memory>

namespace Tristeon
{
	class Layer;
	class Scene
	{
	public:
		Layer* getLayer(unsigned int index);
		unsigned int getLayerCount();
	private:
		std::vector<std::unique_ptr<Layer>> layers;
	};
}