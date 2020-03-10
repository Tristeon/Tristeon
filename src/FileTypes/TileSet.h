#pragma once
#include <memory>
#include <Rendering/Texture.h>
#include <Serialization/Serializable.h>
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	struct Tile
	{
		//TODO: Tile info
	};
	
	class TileSet : public Serializable
	{
		REGISTER_TYPE_H(TileSet)
	public:
		explicit TileSet();
		explicit TileSet(std::string const& imagePath, unsigned int const& cols, unsigned int const& rows, Tile tileInfo[], int const& tileRenderWidth = -1, int const& tileRenderHeight = -1);
		json serialize() override;
		void deserialize(json j) override;
		
		unsigned int cols = 0;
		unsigned int rows = 0;
		unsigned int tileRenderWidth;
		unsigned int tileRenderHeight;
		std::unique_ptr<Tile[]> tileInfo = nullptr;
		std::unique_ptr<Texture> texture = nullptr;
	};
}
