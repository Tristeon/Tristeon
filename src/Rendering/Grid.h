#pragma once
#include <Math/Vector2.h>
#include <Math/Vector2Int.h>

namespace Tristeon
{
	class Grid
	{
		friend class Renderer;
	public:
		static unsigned int tileWidth();
		static unsigned int tileHeight();

		/**
		 * Snaps the given world position to grid cell positions
		 */
		static Vector2 snap(Vector2 const& position);

		/**
		 * Gets the index of the grid cell by the given world position.
		 */
		static Vector2Int indexByPosition(Vector2 const& position);

		/**
		 * Gets the index of the grid cell by the given world position.
		 */
		static Vector2Int indexByPosition(float const& wx, float const& wy);

		/**
		 * Gets the world position of the grid cell at the given index.
		 */
		static Vector2 positionByIndex(Vector2Int const& index);

		/**
		 * Gets the world position of the grid cell at the given index.
		 */
		static Vector2 positionByIndex(int const& ix, int const& iy);

	private:
		static void render();
	};
}
