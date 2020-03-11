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
		explicit TileSet(std::string const& imagePath, 
			uint const& cols, 
			uint const& rows, 
			Tile tileInfo[], 
			int const& tileRenderWidth = -1, 
			int const& tileRenderHeight = -1,

			uint const& spacingLeft = 0,
			uint const& spacingRight = 0,
			uint const& spacingTop = 0,
			uint const& spacingBottom = 0,
			uint const& horizontalSpacing = 0, 
			uint const& verticalSpacing = 0
		);

		json serialize() override;
		void deserialize(json j) override;
		
		unsigned int cols = 0;
		unsigned int rows = 0;
		unsigned int tileRenderWidth = 0;
		unsigned int tileRenderHeight = 0;

		unsigned int spacingLeft = 0;
		unsigned int spacingRight = 0;
		unsigned int spacingTop = 0;
		unsigned int spacingBottom = 0;
		
		unsigned int horizontalSpacing = 0;
		unsigned int verticalSpacing = 0;
		
		std::unique_ptr<Tile[]> tileInfo = nullptr;
		std::unique_ptr<Texture> texture = nullptr;
	};
}
