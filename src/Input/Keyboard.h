#pragma once
#include <map>

#ifdef TRISTEON_EDITOR
namespace TristeonEditor { class EditorWindow; }
#endif

namespace Tristeon
{
	class Window;
	class Engine;

#ifndef TRISTEON_EDITOR
	class GameWindow;
#endif
	
	/**
	 * Interface to interact with the OS Keyboard.
	 *
	 * To get keyboard input, use pressed(), held(), released() and more.
	 */
	class Keyboard
	{
		friend Window;
		friend Engine;

#ifdef TRISTEON_EDITOR
		friend TristeonEditor::EditorWindow;
#else
		friend GameWindow;
#endif
		
	public:
		enum Key {
			Unknown = -1,
			Space = 32,
			Apostrophe = 39, /* ' */
			Comma = 44, /* , */
			Minus = 45, /* - */
			Period = 46, /* . */
			Slash = 47, /* / */

			Number0 = 48,
			Number1 = 49,
			Number2 = 50,
			Number3 = 51,
			Number4 = 52,
			Number5 = 53,
			Number6 = 54,
			Number7 = 55,
			Number8 = 56,
			Number9 = 57,

			Semicolon = 59, /* ; */
			Equals = 61, /* = */
			
			A = 65,
			B = 66,
			C = 67,
			D = 68,
			E = 69,
			F = 70,
			G = 71,
			H = 72,
			I = 73,
			J = 74,
			K = 75,
			L = 76,
			M = 77,
			N = 78,
			O = 79,
			P = 80,
			Q = 81,
			R = 82,
			S = 83,
			T = 84,
			U = 85,
			V = 86,
			W = 87,
			X = 88,
			Y = 89,
			Z = 90,
			
			LeftBracket = 91, /* [ */
			Backslash = 92, /* \ */
			RightBracket = 93, /* ] */
			GraveAccent = 96, /* ` */
			
			Escape = 256,
			Enter = 257,
			Tab = 258,
			Backspace = 259,
			Insert = 260,
			Delete = 261,
			
			Right = 262,
			Left = 263,
			Down = 264,
			Up = 265,
			
			PageUp = 266,
			PageDown = 267,
			
			Home = 268,
			End = 269,

			CapsLock = 280,
			ScrollLock = 281,
			NumLock = 282,

			PrintScreen = 283,
			Pause = 284,
			
			F1 = 290,
			F2 = 291,
			F3 = 292,
			F4 = 293,
			F5 = 294,
			F6 = 295,
			F7 = 296,
			F8 = 297,
			F9 = 298,
			F10 = 299,
			F11 = 300,
			F12 = 301,
			F13 = 302,
			F14 = 303,
			F15 = 304,
			F16 = 305,
			F17 = 306,
			F18 = 307,
			F19 = 308,
			F20 = 309,
			F21 = 310,
			F22 = 311,
			F23 = 312,
			F24 = 313,
			F25 = 314,

			Shift = 340,
			Control = 341,
			Alt = 342,
			Super = 343,
			
			LeftSuper = 343,
			RightSuper = 347,
			
			Menu = 348,
			Last = Menu
		};

		/**
		 * Was this key pressed on the current frame?
		 */
		static bool pressed(Key const& key);

		/**
		 * Was this key held during the current frame?
		 */
		static bool held(Key const& key);

		/**
		 * Was this key released on the current frame?
		 */
		static bool released(Key const& key);

	private:
		static void onPress(Key const& key);
		static void onRelease(Key const& key);

		/**
		 * Clears frame-based data structures.
		 * Called every frame by the main Engine loop.
		 */
		static void reset();
		/**
		 * Clears all input.
		 */
		static void clearAll();

		static std::map<int, bool> keys;
		static std::map<int, bool> keysPressed;
		static std::map<int, bool> keysReleased;
	};
}
