#pragma once
#include "Layer.h"

#include <FileTypes/TileSet.h>
#include <Rendering/Shader.h>

#include "TypeDefinitions.h"

namespace Tristeon
{
	class TileLayer : public Layer
	{
	public:
		TileLayer();
		virtual void render(Renderer* renderer, Scene* scene) override;
	private:
		Unique<TileSet> tileSet = nullptr;
		Unique<Shader> shader;
		Unique<int[]> data = nullptr;
		int width = 0, height = 0;
	};
}