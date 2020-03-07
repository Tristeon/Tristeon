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
		explicit TileSet(std::string const& imagePath, unsigned int const& width, unsigned int const& height, Tile tileInfo[]);
		json serialize() override;
		void deserialize(json j) override;
		
		unsigned int width = 0;
		unsigned int height = 0;
		std::unique_ptr<Tile[]> tileInfo = nullptr;
		std::unique_ptr<Texture> texture = nullptr;
	};
}
