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

		/**
		 * The width of the level defined by this tile layer.
		 */
		uint width() const { return w; }
		/**
		 * The height of the level defined by this tile layer.
		 */
		uint height() const { return h; }

		/**
		 * The width at which tiles are rendered.
		 */
		uint renderWidth() const { return tileRenderWidth; }
		/**
		 * Sets the tile render width.
		 */
		void renderWidth(const uint& value) { tileRenderWidth = value; isDirty = true; }

		/**
		 * The height at which tiles are rendered.
		 */
		uint renderHeight() const { return tileRenderHeight; }
		/**
		 * Sets the tile render height.
		 */
		void renderHeight(const uint& value) { tileRenderHeight = value; isDirty = true; }
		
		/**
		 * Sets the tile at x, y to the given value.
		 *
		 * \exception invalid_argument Throws if x or y is less than 0
		 * \exception out_of_range Throws if x is more than width() or y is more than height()
		 */
		void tile(int const& x, int const& y, Tile const& value);

		/**
		 * Sets the tile at coords.x, coords.y to the given value.
		 *
		 * \exception invalid_argument Throws if coords.x or coords.y is less than 0
		 * \exception out_of_range Throws if coords.x is more than width() or coords.y is more than height()
		 */
		void tile(Vector2Int const& coords, Tile const& value);

		/**
		 * Gets the tile at x, y.
		 * 
		 * \exception invalid_argument Throws if x or y is less than 0
		 * \exception out_of_range Throws if x is more than width() or y is more than height()
		 */
		Tile tile(int const& x, int const& y) const;

		/**
		 * Gets the tile at coords.x, coords.y.
		 *
		 * \exception invalid_argument Throws if coords.x or coords.y is less than 0
		 * \exception out_of_range Throws if coords.x is more than width() or coords.y is more than height()
		 */
		Tile tile(Vector2Int const& coords);

		/**
		 * Gets the tileset with the given ID.
		 */
		TileSet* tileset(int id);

		Vector2Int tileRenderSize() const { return { (int)tileRenderWidth, (int)tileRenderHeight }; };
		void tileRenderSize(Vector2Int const& value) { renderWidth(value.x); renderHeight(value.y); };

	protected:
		void render(Renderer* renderer, Scene* scene) override;
	private:
		GLuint tbo = 0;
		GLuint tbo_tex = 0;

		Unique<Shader> shader;

		Unique<Tile[]> tiles = nullptr;
		Vector<TileSet*> tilesets;

		unsigned int w = 0, h = 0;
		unsigned int tileRenderWidth = 0;
		unsigned int tileRenderHeight = 0;

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
