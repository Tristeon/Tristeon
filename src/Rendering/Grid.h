#pragma once
#include <Math/Vector2.h>
#include <Math/Vector2Int.h>

namespace Tristeon
{
	/**
	 * An interface for interacting with the project-wide 2D grid in Tristeon.
	 */
	class Grid
	{
		friend class Renderer;
	public:
		/**
		 * The width of the tiles in pixels.
		 */
		[[nodiscard]] static unsigned int tileWidth();
		/**
		 * The height of the tiles in pixels.
		 */
		[[nodiscard]] static unsigned int tileHeight();
		/**
		 * The size of the tiles in pixels.
		 */
		[[nodiscard]] static Vector2Int tileSize();

		/**
		 * Snaps the given world position to grid cell positions.
		 */
		[[nodiscard]] static Vector2 snap(const Vector2& position);

		/**
		 * Gets the index of the grid cell by the given world position.
		 */
		[[nodiscard]] static Vector2Int indexByPosition(const Vector2& position);

		/**
		 * Gets the index of the grid cell by the given world position.
		 */
		[[nodiscard]] static Vector2Int indexByPosition(const float& wx, const float& wy);

		/**
		 * Gets the world position of the grid cell at the given index.
		 */
		[[nodiscard]] static Vector2 positionByIndex(const Vector2Int& index);

		/**
		 * Gets the world position of the grid cell at the given index.
		 */
		[[nodiscard]] static Vector2 positionByIndex(const int& ix, const int& iy);

	private:
		static void render();
	};
}