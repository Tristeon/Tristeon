#pragma once
#include <json.h>
#include <Serialization/Typename.h>

namespace Tristeon
{
	/**
	 * Colour defines a Colour using RGBA values between 0 and 1.
	 */
	struct Colour final
	{
		/**
		 * The red value of the Colour (0...1).
		 */
		float r = 1;
		/**
		 * The green value of the Colour (0...1).
		 */
		float g = 1;
		/**
		 * The blue value of the Colour (0...1).
		 */
		float b = 1;
		/**
		 * The alpha/transparency value of the Colour (0...1).
		 */
		float a = 1;

		/**
		 * Creates a colour with the given rgba values.
		 */
		constexpr Colour(const float& r = 1, const float& g = 1, const float& b = 1, const float& a = 1) : r(r), g(g), b(b), a(a) { }

		/**
		 * (1, 1, 1, 1)
		 */
		static Colour white() { return { 1, 1, 1, 1 }; }
		/**
		 * (0, 0, 0, 1)
		 */
		static Colour black() { return { 0, 0, 0, 1 }; }

		/**
		 * (1, 0, 0, 1)
		 */
		static Colour red() { return { 1, 0, 0, 1 }; }
		/**
		 * (0, 1, 0, 1)
		 */
		static Colour green() { return { 0, 1, 0, 1 }; }
		/**
		 * (0, 0, 1, 1)
		 */
		static Colour blue() { return { 0, 0, 1, 1 }; }
		/**
		 * (1, 1, 0, 1)
		 */
		static Colour yellow() { return { 1, 1, 0, 1 }; }
		/**
		 * (1, 0, 1, 1)
		 */
		static Colour purple() { return { 1, 0, 1, 1 }; }
	};

	inline void to_json(nlohmann::json& j, const Colour& p) {
		j["r"] = p.r;
		j["g"] = p.g;
		j["b"] = p.b;
		j["a"] = p.a;
		j["typeID"] = TRISTEON_TYPENAME(Colour);
	}

	inline void from_json(const nlohmann::json& j, Colour& p) {
		p.r = j.value("r", 1.0f);
		p.g = j.value("g", 1.0f);
		p.b = j.value("b", 1.0f);
		p.a = j.value("a", 1.0f);
	}

	static_assert(sizeof(Colour) == sizeof(float) * 4, "Colour can not be bigger than 4 floats!");
}
