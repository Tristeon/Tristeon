#pragma once
#include "ICallback.h"

namespace Tristeon
{
	/**
	 * Inherit from this class to receive draw gizmo callbacks.
	 * Draw gizmos is called once per frame, gizmos are rendered at the end of the frame and cleared after.
	 *
	 * The inherited class does NOT have to be an actor or behaviour, any class will work.
	 */
	class IDrawGizmos : ICallback<IDrawGizmos>
	{
	public:
		/**
		 * Draw gizmos is called once per frame, gizmos are rendered at the end of the frame and cleared after.
		 *
		 * Use the Gizmos class in this function.
		 */
		virtual void drawGizmos() = 0;
	};
}