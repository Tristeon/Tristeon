#pragma once

namespace Tristeon
{
	/**
	 * Time is a class in Tristeon which allows you to get time information in Tristeon, such as the frame's delta time.
	 */
	class Time
	{
		friend class Engine;
		friend class Project;
	public:
		/**
		 * The delta time of a single frame in milliseconds.
		 */
		static float deltaTime();

		/**
		 * The delta time of the physics update in milliseconds.
		 *
		 * This value is constant, it is set in the project settings and it can't be modified in runtime.
		 * The physics update and fixedUpdate() will try to closely match this value.
		 */
		static float fixedDeltaTime();

		/**
		 * The amount of frames per second that the main game loop is running at.
		 */
		static unsigned int fps();

	private:
		static float m_deltaTime;
		static unsigned int m_fps;
	};
}