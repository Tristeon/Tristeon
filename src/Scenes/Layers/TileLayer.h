#pragma once
#include "Layer.h"

#include <FileTypes/TileSet.h>
#include <Rendering/Shader.h>

namespace Tristeon
{
	class TileLayer : public Layer
	{
	public:
		TileLayer();
		virtual void render(Scene* scene) override;
	private:
		std::unique_ptr<TileSet> tileSet = nullptr;
		Shader shader;
		std::unique_ptr<int[]> data = nullptr;
		int width = 0, height = 0;
	};
}