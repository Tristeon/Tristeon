#pragma once
#include "Layer.h"
#include "Tile.h"

#include <FileTypes/TileSet.h>
#include <Rendering/Shader.h>

#include <TypeDefinitions.h>
#include <Serialization/TypeRegister.h>
#include <Registers/LayerRegister.h>

#include <Math/Vector2Int.h>

#include <box2d/b2_fixture.h>

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
		REGISTER_LAYER_H(TileLayer);
		REGISTER_TYPE_H(TileLayer);
		friend SceneManager;
		friend class CollisionListener;

	public:
		TileLayer();
		virtual ~TileLayer();

		json serialize() override;
		void deserialize(json j) override;

#pragma region Core
		/**
		 * The width of the level defined by this tile layer.
		 */
		uint width() const { return _width; }
		/**
		 * The height of the level defined by this tile layer.
		 */
		uint height() const { return _height; }
#pragma endregion

#pragma region TileSets
		/**
		 * Gets the tileset with the given ID.
		 */
		TileSet* tileset(int const& id);

		/**
		 * The width at which tiles are rendered.
		 */
		uint tileRenderWidth() const { return _tileRenderWidth; }
		/**
		 * Sets the tile render width.
		 */
		void setTileRenderWidth(const uint& value) { _tileRenderWidth = value; isDirty = true; }

		/**
		 * The height at which tiles are rendered.
		 */
		uint renderHeight() const { return _tileRenderHeight; }
		/**
		 * Sets the tile render height.
		 */
		void setRenderHeight(const uint& value) { _tileRenderHeight = value; isDirty = true; }
		
		/**
		 * The size of the tiles when rendered.
		 */
		Vector2Int tileRenderSize() const { return { (int)_tileRenderWidth, (int)_tileRenderHeight }; }
		/**
		 * Sets the size of the tiles when rendered.
		 */
		void setTileRenderSize(Vector2Int const& value) { setTileRenderWidth(value.x); setRenderHeight(value.y); };
#pragma endregion

#pragma region Tile Modification
		/**
		 * Sets the tile at x, y to the given value.
		 *
		 * \exception invalid_argument Throws if x or y is less than 0
		 * \exception out_of_range Throws if x is more than width() or y is more than height()
		 */
		void setTileByIndex(int const& ix, int const& iy, Tile const& value);

		/**
		 * Sets the tile at index.x, index.y to the given value.
		 *
		 * \exception invalid_argument Throws if index.x or index.y is less than 0
		 * \exception out_of_range Throws if index.x is more than width() or index.y is more than height()
		 */
		void setTileByIndex(Vector2Int const& index, Tile const& value);

		/**
		 * Sets the tile at the given world position to the given value.
		 *
		 * \exception invalid_argument Throws if position is under 0
		 * \exception out_of_range Throws if the position is outside of the tilemap
		 */
		void setTileByPosition(Vector2 const& position, Tile const& value);

		/**
		 * Sets the tile at the given world position to the given value.
		 *
		 * \exception invalid_argument Throws if position is under 0
		 * \exception out_of_range Throws if the position is outside of the tilemap
		 */
		void setTileByPosition(float const& wx, float const& wy, Tile const& value);

		/**
		 * Gets the tile at x, y.
		 * 
		 * \exception invalid_argument Throws if x or y is less than 0
		 * \exception out_of_range Throws if x is more than width() or y is more than height()
		 */
		Tile tileByIndex(int const& ix, int const& iy) const;

		/**
		 * Gets the tile at coords.x, coords.y.
		 *
		 * \exception invalid_argument Throws if coords.x or coords.y is less than 0
		 * \exception out_of_range Throws if coords.x is more than width() or coords.y is more than height()
		 */
		Tile tileByIndex(Vector2Int const& index) const;

		/**
		 * Gets the tile at the given world position.
		 *
		 * \exception invalid_argument Throws if position is under 0
		 * \exception out_of_range Throws if the position is outside of the tilemap
		 */
		Tile tileByPosition(Vector2 const& position) const;

		/**
		 * Gets the tile at the given world position.
		 *
		 * \exception invalid_argument Throws if position is under 0
		 * \exception out_of_range Throws if the position is outside of the tilemap
		 */
		Tile tileByPosition(float const& wx, float const& wy) const;

		/**
		 * Gets the index of the tile by the given world position.
		 */
		Vector2Int indexByPosition(Vector2 const& position) const;

		/**
		 * Gets the index of the tile by the given world position.
		 */
		Vector2Int indexByPosition(float const& wx, float const& wy) const;
		
		/**
		 * Returns true if the given index is within the bounds of the TileLayer
		 */
		bool checkBoundsByIndex(Vector2Int const& index) const;

		/**
		 * Returns true if the world position is within the bounds of the TileLayer
		 */
		bool checkBoundsByPosition(Vector2 const& position) const;
#pragma endregion
		
	protected:
		void render(Renderer* renderer, Scene* scene) override;
	private:
		GLuint tbo = 0;
		GLuint tbo_tex = 0;

		Unique<Shader> shader;

		Unique<Tile[]> tiles = nullptr;
		Vector<TileSet*> tilesets;

		unsigned int _width = 0, _height = 0;
		unsigned int _tileRenderWidth = 0;
		unsigned int _tileRenderHeight = 0;

		bool isDirty = false;

		/**
		 * Creates a texture buffer object.
		 *
		 * A texture buffer object is a 1D buffer, used to send big arrays of data to the GPU.
		 * This TBO is used to send the Level data to the tile shader.
		 */
		void createTBO();
		
		/**
		 * Creates box colliders for all tiles that have the hasCollider flag set to true in their TileSet.
		 * Currently doesn't support other shapes.
		 */
		void createColliders();

		std::map<Vector2Int, b2Fixture*> fixtures;
	};
}
