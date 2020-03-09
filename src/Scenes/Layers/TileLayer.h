#pragma once
#include "Layer.h"

#include <FileTypes/TileSet.h>
#include <Rendering/Shader.h>

#include <TypeDefinitions.h>
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	class SceneManager;
	
	class TileLayer : public Layer
	{
		REGISTER_TYPE_H(TileLayer)
		friend SceneManager;
	public:
		TileLayer();
		~TileLayer();
		json serialize() override;
		void deserialize(json j) override;

		virtual void render(Renderer* renderer, Scene* scene) override;
	private:
		GLuint tbo;
		GLuint tbo_tex;
		
		Unique<TileSet> tileSet = nullptr;
		Unique<Shader> shader;
		Unique<int[]> data = nullptr;
		int width = 0, height = 0;
	};
}