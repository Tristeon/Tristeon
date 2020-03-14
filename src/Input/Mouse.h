#pragma once
#include <QMouseEvent>
#include "Math/Vector2Int.h"

namespace Tristeon
{
	//Slightly adjusted Qt::MouseButton, copied into the Tristeon namespace to keep the API consistent
	enum MouseButton {
		LeftButton = 1,
		RightButton = 2,
		MidButton = 3,
		MiddleButton = MidButton,
		BackButton = 4,
		XButton1 = BackButton,
		ExtraButton1 = XButton1,
		ForwardButton = 5,
		XButton2 = ForwardButton,
		ExtraButton2 = ForwardButton,
		TaskButton = 6,
		ExtraButton3 = 7,
		ExtraButton4 = 8,
		ExtraButton5 = 9,
		ExtraButton6 = 10,
		ExtraButton7 = 11,
		ExtraButton8 = 12,
		ExtraButton9 = 13,
		ExtraButton10 = 14,
		ExtraButton11 = 15,
		ExtraButton12 = 16,
		ExtraButton13 = 17,
		ExtraButton14 = 18,
		ExtraButton15 = 19,
		ExtraButton16 = 20,
		ExtraButton17 = 21,
		ExtraButton18 = 22,
		ExtraButton19 = 23,
		ExtraButton20 = 24,
		ExtraButton21 = 25,
		ExtraButton22 = 26,
		ExtraButton23 = 27,
		ExtraButton24 = 28,
		MaxMouseButton = ExtraButton24,
	};
	
	class Window;
	class Engine;
	class Mouse final
	{
		friend Window;
		friend Engine;
		
		static bool pressed(MouseButton button);
		static bool held(MouseButton button);
		static bool released(MouseButton button);
		static bool doubleClicked(MouseButton button);
		static Vector2 position();
	private:
		static void onPress(QMouseEvent event);
		static void onRelease(QMouseEvent event);
		static void onDoubleClick(QMouseEvent event);
		static void onMove(QMouseEvent event);

		static void reset();
		
		static bool buttons[];
		static bool buttonsPressed[];
		static bool buttonsReleased[];
		static bool buttonsDoubleClicked[];

		static Vector2Int mousePos;
	};
}
