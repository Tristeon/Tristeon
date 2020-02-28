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
		virtual void render(Renderer* renderer, Scene* scene) override;
	private:
		std::unique_ptr<TileSet> tileSet = nullptr;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<int[]> data = nullptr;
		int width = 0, height = 0;
	};
}