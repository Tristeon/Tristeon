#pragma once

namespace Tristeon
{
	/**
	 * A wrapper around a GL framebuffer.
	 * It stores both the framebuffer ID and the viewport's information.
	 */
	struct Framebuffer
	{
		/**
		 * The OpenGL framebuffer ID.
		 */
		unsigned int framebufferID = 0;

		struct {
			/**
			 * The horizontal coordinate of the lower left corner in pixels.
			 */
			unsigned int x = 0;
			/**
			 * The vertical coordinate of the lower left corner in pixels.
			 */
			unsigned int y = 0;

			/**
			 * The width of the viewport in pixels.
			 */
			unsigned int width = 0;
			/**
			 * The height of the viewport in pixels.
			 */
			unsigned int height = 0;
		} viewport;

		/**
		 * Binds the framebuffer and viewport for rendering usage.
		 */
		void bind();
	};
}