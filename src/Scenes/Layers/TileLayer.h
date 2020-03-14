#pragma once
#include "Layer.h"

#include <FileTypes/TileSet.h>
#include <Rendering/Shader.h>

#include <TypeDefinitions.h>
#include <Serialization/TypeRegister.h>

#include <Math/Vector2Int.h>

namespace Tristeon
{
	class SceneManager;

	/**
	 * The tilelayer renders tiles on a 2D grid using a given tileset.
	 *
	 * Tiles are rendered on the GPU using a full-screen shader, so performance cost should be minimal.
	 * Tile-specific behaviour (such as collisions, animations) can be defined through the Tile class in TileSet.
	 *
	 * This class can be used as an interface to change tile values within the level.
	 * Tiles are represented by a single index, read on the tileset texture from top left to bottom right.
	 * To convert said index to tileset x,y coordinates, you can use the tileSet()'s interface.
	 */
	class TileLayer : public Layer
	{
		REGISTER_TYPE_H(TileLayer);
		friend SceneManager;
	public:
		TileLayer();
		~TileLayer();

		json serialize() override;
		void deserialize(json j) override;

		/**
		 * The width of the level defined by this tile layer.
		 */
		uint width() const { return w; }
		/**
		 * The height of the level defined by this tile layer.
		 */
		uint height() const { return h; }

		/**
		 * Gets a reference to a tile at coords.x, coords.y.
		 * This can be used to either get or set the value of the tile.
		 *
		 * \exception invalid_argument Throws if coords.x or coords.y is less than 0
		 * \exception out_of_range Throws if coords.x is more than width() or coords.y is more than height()
		 */
		int& operator[](Vector2Int const& coords);

		/**
		 * Sets the tile at x, y to the given value.
		 *
		 * \exception invalid_argument Throws if x or y is less than 0
		 * \exception out_of_range Throws if x is more than width() or y is more than height()
		 */
		void tile(int const& x, int const& y, int const& value);

		/**
		 * Sets the tile at coords.x, coords.y to the given value.
		 *
		 * \exception invalid_argument Throws if coords.x or coords.y is less than 0
		 * \exception out_of_range Throws if coords.x is more than width() or coords.y is more than height()
		 */
		void tile(Vector2Int const& coords, int const& value);

		/**
		 * Gets the tile at x, y.
		 * 
		 * \exception invalid_argument Throws if x or y is less than 0
		 * \exception out_of_range Throws if x is more than width() or y is more than height()
		 */
		int tile(int const& x, int const& y);

		/**
		 * Gets the tile at coords.x, coords.y.
		 *
		 * \exception invalid_argument Throws if coords.x or coords.y is less than 0
		 * \exception out_of_range Throws if coords.x is more than width() or coords.y is more than height()
		 */
		int tile(Vector2Int const& coords);
	protected:
		void render(Renderer* renderer, Scene* scene) override;

	private:
		GLuint tbo = 0;
		GLuint tbo_tex = 0;

		Unique<TileSet> tileSet = nullptr;
		Unique<Shader> shader;
		Unique<int[]> data = nullptr;
		unsigned int w = 0, h = 0;

		bool isDirty = false;

		/**
		 * Creates a texture buffer object.
		 *
		 * A texture buffer object is a 1D buffer, used to send big arrays of data to the GPU.
		 * This TBO is used to send the Level data to the tile shader.
		 */
		void createTBO();
	};
}