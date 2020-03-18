#pragma once
#include <map>
#include <QKeyEvent>

namespace Tristeon
{
	class Window;
	class Engine;

	/**
	 * Interface to interact with the OS Keyboard.
	 *
	 * To get keyboard input, use pressed(), held(), released() and more.
	 */
	class Keyboard
	{
		friend Window;
		friend Engine;

	public:
		//Qt::Key copied into the Tristeon namespace to keep API consistent
		enum Key {
			Escape = 0x01000000,                // misc keys
			Tab = 0x01000001,
			Backtab = 0x01000002,
			Backspace = 0x01000003,
			Return = 0x01000004,
			Enter = 0x01000005,
			Insert = 0x01000006,
			Delete = 0x01000007,
			Pause = 0x01000008,
			Print = 0x01000009,               // print screen
			SysReq = 0x0100000a,
			Clear = 0x0100000b,
			Home = 0x01000010,                // cursor movement
			End = 0x01000011,
			Left = 0x01000012,
			Up = 0x01000013,
			Right = 0x01000014,
			Down = 0x01000015,
			PageUp = 0x01000016,
			PageDown = 0x01000017,
			Shift = 0x01000020,                // modifiers
			Control = 0x01000021,
			Meta = 0x01000022,
			Alt = 0x01000023,
			CapsLock = 0x01000024,
			NumLock = 0x01000025,
			ScrollLock = 0x01000026,
			F1 = 0x01000030,                // function keys
			F2 = 0x01000031,
			F3 = 0x01000032,
			F4 = 0x01000033,
			F5 = 0x01000034,
			F6 = 0x01000035,
			F7 = 0x01000036,
			F8 = 0x01000037,
			F9 = 0x01000038,
			F10 = 0x01000039,
			F11 = 0x0100003a,
			F12 = 0x0100003b,
			F13 = 0x0100003c,
			F14 = 0x0100003d,
			F15 = 0x0100003e,
			F16 = 0x0100003f,
			F17 = 0x01000040,
			F18 = 0x01000041,
			F19 = 0x01000042,
			F20 = 0x01000043,
			F21 = 0x01000044,
			F22 = 0x01000045,
			F23 = 0x01000046,
			F24 = 0x01000047,
			F25 = 0x01000048,                // F25 .. F35 only on X11
			F26 = 0x01000049,
			F27 = 0x0100004a,
			F28 = 0x0100004b,
			F29 = 0x0100004c,
			F30 = 0x0100004d,
			F31 = 0x0100004e,
			F32 = 0x0100004f,
			F33 = 0x01000050,
			F34 = 0x01000051,
			F35 = 0x01000052,
			Super_L = 0x01000053,                 // extra keys
			Super_R = 0x01000054,
			Menu = 0x01000055,
			Hyper_L = 0x01000056,
			Hyper_R = 0x01000057,
			Help = 0x01000058,
			Direction_L = 0x01000059,
			Direction_R = 0x01000060,
			Space = 0x20,                // 7 bit printable ASCII
			Any = Space,
			Exclam = 0x21,
			QuoteDbl = 0x22,
			NumberSign = 0x23,
			Dollar = 0x24,
			Percent = 0x25,
			Ampersand = 0x26,
			Apostrophe = 0x27,
			ParenLeft = 0x28,
			ParenRight = 0x29,
			Asterisk = 0x2a,
			Plus = 0x2b,
			Comma = 0x2c,
			Minus = 0x2d,
			Period = 0x2e,
			Slash = 0x2f,
			Alpha0 = 0x30,
			Alpha1 = 0x31,
			Alpha2 = 0x32,
			Alpha3 = 0x33,
			Alpha4 = 0x34,
			Alpha5 = 0x35,
			Alpha6 = 0x36,
			Alpha7 = 0x37,
			Alpha8 = 0x38,
			Alpha9 = 0x39,
			Colon = 0x3a,
			Semicolon = 0x3b,
			Less = 0x3c,
			Equal = 0x3d,
			Greater = 0x3e,
			Question = 0x3f,
			At = 0x40,
			A = 0x41,
			B = 0x42,
			C = 0x43,
			D = 0x44,
			E = 0x45,
			F = 0x46,
			G = 0x47,
			H = 0x48,
			I = 0x49,
			J = 0x4a,
			K = 0x4b,
			L = 0x4c,
			M = 0x4d,
			N = 0x4e,
			O = 0x4f,
			P = 0x50,
			Q = 0x51,
			R = 0x52,
			S = 0x53,
			T = 0x54,
			U = 0x55,
			V = 0x56,
			W = 0x57,
			X = 0x58,
			Y = 0x59,
			Z = 0x5a,
			BracketLeft = 0x5b,
			Backslash = 0x5c,
			BracketRight = 0x5d,
			AsciiCircum = 0x5e,
			Underscore = 0x5f,
			QuoteLeft = 0x60,
			BraceLeft = 0x7b,
			Bar = 0x7c,
			BraceRight = 0x7d,
			AsciiTilde = 0x7e,

			nobreakspace = 0x0a0,
			exclamdown = 0x0a1,
			cent = 0x0a2,
			sterling = 0x0a3,
			currency = 0x0a4,
			yen = 0x0a5,
			brokenbar = 0x0a6,
			section = 0x0a7,
			diaeresis = 0x0a8,
			copyright = 0x0a9,
			ordfeminine = 0x0aa,
			guillemotleft = 0x0ab,        // left angle quotation mark
			notsign = 0x0ac,
			hyphen = 0x0ad,
			registered = 0x0ae,
			macron = 0x0af,
			degree = 0x0b0,
			plusminus = 0x0b1,
			twosuperior = 0x0b2,
			threesuperior = 0x0b3,
			acute = 0x0b4,
			mu = 0x0b5,
			paragraph = 0x0b6,
			periodcentered = 0x0b7,
			cedilla = 0x0b8,
			onesuperior = 0x0b9,
			masculine = 0x0ba,
			guillemotright = 0x0bb,        // right angle quotation mark
			onequarter = 0x0bc,
			onehalf = 0x0bd,
			threequarters = 0x0be,
			questiondown = 0x0bf,
			Agrave = 0x0c0,
			Aacute = 0x0c1,
			Acircumflex = 0x0c2,
			Atilde = 0x0c3,
			Adiaeresis = 0x0c4,
			Aring = 0x0c5,
			AE = 0x0c6,
			Ccedilla = 0x0c7,
			Egrave = 0x0c8,
			Eacute = 0x0c9,
			Ecircumflex = 0x0ca,
			Ediaeresis = 0x0cb,
			Igrave = 0x0cc,
			Iacute = 0x0cd,
			Icircumflex = 0x0ce,
			Idiaeresis = 0x0cf,
			ETH = 0x0d0,
			Ntilde = 0x0d1,
			Ograve = 0x0d2,
			Oacute = 0x0d3,
			Ocircumflex = 0x0d4,
			Otilde = 0x0d5,
			Odiaeresis = 0x0d6,
			multiply = 0x0d7,
			Ooblique = 0x0d8,
			Ugrave = 0x0d9,
			Uacute = 0x0da,
			Ucircumflex = 0x0db,
			Udiaeresis = 0x0dc,
			Yacute = 0x0dd,
			THORN = 0x0de,
			ssharp = 0x0df,
			division = 0x0f7,
			ydiaeresis = 0x0ff,

			// International input method support (X keycode - 0xEE00, the
			// definition follows Qt/Embedded 2.3.7) Only interesting if
			// you are writing your own input method

			// International & multi-key character composition
			AltGr = 0x01001103,
			Multi_key = 0x01001120,  // Multi-key character compose
			Codeinput = 0x01001137,
			SingleCandidate = 0x0100113c,
			MultipleCandidate = 0x0100113d,
			PreviousCandidate = 0x0100113e,

			// Misc Functions
			Mode_switch = 0x0100117e,  // Character set switch

			// Japanese keyboard support
			Kanji = 0x01001121,  // Kanji, Kanji convert
			Muhenkan = 0x01001122,  // Cancel Conversion
			//Henkan_Mode         = 0x01001123,  // Start/Stop Conversion
			Henkan = 0x01001123,  // Alias for Henkan_Mode
			Romaji = 0x01001124,  // to Romaji
			Hiragana = 0x01001125,  // to Hiragana
			Katakana = 0x01001126,  // to Katakana
			Hiragana_Katakana = 0x01001127,  // Hiragana/Katakana toggle
			Zenkaku = 0x01001128,  // to Zenkaku
			Hankaku = 0x01001129,  // to Hankaku
			Zenkaku_Hankaku = 0x0100112a,  // Zenkaku/Hankaku toggle
			Touroku = 0x0100112b,  // Add to Dictionary
			Massyo = 0x0100112c,  // Delete from Dictionary
			Kana_Lock = 0x0100112d,  // Kana Lock
			Kana_Shift = 0x0100112e,  // Kana Shift
			Eisu_Shift = 0x0100112f,  // Alphanumeric Shift
			Eisu_toggle = 0x01001130,  // Alphanumeric toggle

			// Korean keyboard support
			//
			// In fact, many Korean users need only 2 keys, Key_Hangul and
			// Key_Hangul_Hanja. But rest of the keys are good for future.

			Hangul = 0x01001131,  // Hangul start/stop(toggle)
			Hangul_Start = 0x01001132,  // Hangul start
			Hangul_End = 0x01001133,  // Hangul end, English start
			Hangul_Hanja = 0x01001134,  // Start Hangul->Hanja Conversion
			Hangul_Jamo = 0x01001135,  // Hangul Jamo mode
			Hangul_Romaja = 0x01001136,  // Hangul Romaja mode
			Hangul_Jeonja = 0x01001138,  // Jeonja mode
			Hangul_Banja = 0x01001139,  // Banja mode
			Hangul_PreHanja = 0x0100113a,  // Pre Hanja conversion
			Hangul_PostHanja = 0x0100113b,  // Post Hanja conversion
			Hangul_Special = 0x0100113f,  // Special symbols

			// dead keys (X keycode - 0xED00 to avoid the conflict)
			Dead_Grave = 0x01001250,
			Dead_Acute = 0x01001251,
			Dead_Circumflex = 0x01001252,
			Dead_Tilde = 0x01001253,
			Dead_Macron = 0x01001254,
			Dead_Breve = 0x01001255,
			Dead_Abovedot = 0x01001256,
			Dead_Diaeresis = 0x01001257,
			Dead_Abovering = 0x01001258,
			Dead_Doubleacute = 0x01001259,
			Dead_Caron = 0x0100125a,
			Dead_Cedilla = 0x0100125b,
			Dead_Ogonek = 0x0100125c,
			Dead_Iota = 0x0100125d,
			Dead_Voiced_Sound = 0x0100125e,
			Dead_Semivoiced_Sound = 0x0100125f,
			Dead_Belowdot = 0x01001260,
			Dead_Hook = 0x01001261,
			Dead_Horn = 0x01001262,
			Dead_Stroke = 0x01001263,
			Dead_Abovecomma = 0x01001264,
			Dead_Abovereversedcomma = 0x01001265,
			Dead_Doublegrave = 0x01001266,
			Dead_Belowring = 0x01001267,
			Dead_Belowmacron = 0x01001268,
			Dead_Belowcircumflex = 0x01001269,
			Dead_Belowtilde = 0x0100126a,
			Dead_Belowbreve = 0x0100126b,
			Dead_Belowdiaeresis = 0x0100126c,
			Dead_Invertedbreve = 0x0100126d,
			Dead_Belowcomma = 0x0100126e,
			Dead_Currency = 0x0100126f,
			Dead_a = 0x01001280,
			Dead_A = 0x01001281,
			Dead_e = 0x01001282,
			Dead_E = 0x01001283,
			Dead_i = 0x01001284,
			Dead_I = 0x01001285,
			Dead_o = 0x01001286,
			Dead_O = 0x01001287,
			Dead_u = 0x01001288,
			Dead_U = 0x01001289,
			Dead_Small_Schwa = 0x0100128a,
			Dead_Capital_Schwa = 0x0100128b,
			Dead_Greek = 0x0100128c,
			Dead_Lowline = 0x01001290,
			Dead_Aboveverticalline = 0x01001291,
			Dead_Belowverticalline = 0x01001292,
			Dead_Longsolidusoverlay = 0x01001293,

			// multimedia/internet keys - ignored by default - see QKeyEvent c'tor
			Back = 0x01000061,
			Forward = 0x01000062,
			Stop = 0x01000063,
			Refresh = 0x01000064,
			VolumeDown = 0x01000070,
			VolumeMute = 0x01000071,
			VolumeUp = 0x01000072,
			BassBoost = 0x01000073,
			BassUp = 0x01000074,
			BassDown = 0x01000075,
			TrebleUp = 0x01000076,
			TrebleDown = 0x01000077,
			MediaPlay = 0x01000080,
			MediaStop = 0x01000081,
			MediaPrevious = 0x01000082,
			MediaNext = 0x01000083,
			MediaRecord = 0x01000084,
			MediaPause = 0x1000085,
			MediaTogglePlayPause = 0x1000086,
			HomePage = 0x01000090,
			Favorites = 0x01000091,
			Search = 0x01000092,
			Standby = 0x01000093,
			OpenUrl = 0x01000094,
			LaunchMail = 0x010000a0,
			LaunchMedia = 0x010000a1,
			Launch0 = 0x010000a2,
			Launch1 = 0x010000a3,
			Launch2 = 0x010000a4,
			Launch3 = 0x010000a5,
			Launch4 = 0x010000a6,
			Launch5 = 0x010000a7,
			Launch6 = 0x010000a8,
			Launch7 = 0x010000a9,
			Launch8 = 0x010000aa,
			Launch9 = 0x010000ab,
			LaunchA = 0x010000ac,
			LaunchB = 0x010000ad,
			LaunchC = 0x010000ae,
			LaunchD = 0x010000af,
			LaunchE = 0x010000b0,
			LaunchF = 0x010000b1,
			MonBrightnessUp = 0x010000b2,
			MonBrightnessDown = 0x010000b3,
			KeyboardLightOnOff = 0x010000b4,
			KeyboardBrightnessUp = 0x010000b5,
			KeyboardBrightnessDown = 0x010000b6,
			PowerOff = 0x010000b7,
			WakeUp = 0x010000b8,
			Eject = 0x010000b9,
			ScreenSaver = 0x010000ba,
			WWW = 0x010000bb,
			Memo = 0x010000bc,
			LightBulb = 0x010000bd,
			Shop = 0x010000be,
			History = 0x010000bf,
			AddFavorite = 0x010000c0,
			HotLinks = 0x010000c1,
			BrightnessAdjust = 0x010000c2,
			Finance = 0x010000c3,
			Community = 0x010000c4,
			AudioRewind = 0x010000c5, // Media rewind
			BackForward = 0x010000c6,
			ApplicationLeft = 0x010000c7,
			ApplicationRight = 0x010000c8,
			Book = 0x010000c9,
			CD = 0x010000ca,
			Calculator = 0x010000cb,
			ToDoList = 0x010000cc,
			ClearGrab = 0x010000cd,
			Close = 0x010000ce,
			Copy = 0x010000cf,
			Cut = 0x010000d0,
			Display = 0x010000d1, // Output switch key
			DOS = 0x010000d2,
			Documents = 0x010000d3,
			Excel = 0x010000d4,
			Explorer = 0x010000d5,
			Game = 0x010000d6,
			Go = 0x010000d7,
			iTouch = 0x010000d8,
			LogOff = 0x010000d9,
			Market = 0x010000da,
			Meeting = 0x010000db,
			MenuKB = 0x010000dc,
			MenuPB = 0x010000dd,
			MySites = 0x010000de,
			News = 0x010000df,
			OfficeHome = 0x010000e0,
			Option = 0x010000e1,
			Paste = 0x010000e2,
			Phone = 0x010000e3,
			Calendar = 0x010000e4,
			Reply = 0x010000e5,
			Reload = 0x010000e6,
			RotateWindows = 0x010000e7,
			RotationPB = 0x010000e8,
			RotationKB = 0x010000e9,
			Save = 0x010000ea,
			Send = 0x010000eb,
			Spell = 0x010000ec,
			SplitScreen = 0x010000ed,
			Support = 0x010000ee,
			TaskPane = 0x010000ef,
			Terminal = 0x010000f0,
			Tools = 0x010000f1,
			Travel = 0x010000f2,
			Video = 0x010000f3,
			Word = 0x010000f4,
			Xfer = 0x010000f5,
			ZoomIn = 0x010000f6,
			ZoomOut = 0x010000f7,
			Away = 0x010000f8,
			Messenger = 0x010000f9,
			WebCam = 0x010000fa,
			MailForward = 0x010000fb,
			Pictures = 0x010000fc,
			Music = 0x010000fd,
			Battery = 0x010000fe,
			Bluetooth = 0x010000ff,
			WLAN = 0x01000100,
			UWB = 0x01000101,
			AudioForward = 0x01000102, // Media fast-forward
			AudioRepeat = 0x01000103, // Toggle repeat mode
			AudioRandomPlay = 0x01000104, // Toggle shuffle mode
			Subtitle = 0x01000105,
			AudioCycleTrack = 0x01000106,
			Time = 0x01000107,
			Hibernate = 0x01000108,
			View = 0x01000109,
			TopMenu = 0x0100010a,
			PowerDown = 0x0100010b,
			Suspend = 0x0100010c,
			ContrastAdjust = 0x0100010d,

			LaunchG = 0x0100010e,
			LaunchH = 0x0100010f,

			TouchpadToggle = 0x01000110,
			TouchpadOn = 0x01000111,
			TouchpadOff = 0x01000112,

			MicMute = 0x01000113,

			Red = 0x01000114,
			Green = 0x01000115,
			Yellow = 0x01000116,
			Blue = 0x01000117,

			ChannelUp = 0x01000118,
			ChannelDown = 0x01000119,

			Guide = 0x0100011a,
			Info = 0x0100011b,
			Settings = 0x0100011c,

			MicVolumeUp = 0x0100011d,
			MicVolumeDown = 0x0100011e,

			New = 0x01000120,
			Open = 0x01000121,
			Find = 0x01000122,
			Undo = 0x01000123,
			Redo = 0x01000124,

			MediaLast = 0x0100ffff,

			// Keypad navigation keys
			Select = 0x01010000,
			Yes = 0x01010001,
			No = 0x01010002,

			// Newer misc keys
			Cancel = 0x01020001,
			Printer = 0x01020002,
			Execute = 0x01020003,
			Sleep = 0x01020004,
			Play = 0x01020005, // Not the same as Key_MediaPlay
			Zoom = 0x01020006,
			Exit = 0x0102000a,

			// Device keys
			Context1 = 0x01100000,
			Context2 = 0x01100001,
			Context3 = 0x01100002,
			Context4 = 0x01100003,
			Call = 0x01100004,      // set absolute state to in a call (do not toggle state)
			Hangup = 0x01100005,    // set absolute state to hang up (do not toggle state)
			Flip = 0x01100006,
			ToggleCallHangup = 0x01100007, // a toggle key for answering, or hanging up, based on current call state
			VoiceDial = 0x01100008,
			LastNumberRedial = 0x01100009,

			Camera = 0x01100020,
			CameraFocus = 0x01100021,

			unknown = 0x01ffffff
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
		static void onPress(QKeyEvent const& event);
		static void onRelease(QKeyEvent const& event);

		/**
		 * Clears frame-based data structures.
		 * Called every frame by the main Engine loop.
		 */
		static void reset();

		static std::map<int, bool> keys;
		static std::map<int, bool> keysPressed;
		static std::map<int, bool> keysReleased;
	};
}
