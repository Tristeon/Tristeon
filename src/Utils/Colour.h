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
		 * The red value of the Colour.
		 */
		float r = 1;
		/**
		 * The green value of the Colour.
		 */
		float g = 1;
		/**
		 * The blue value of the Colour.
		 */
		float b = 1;
		/**
		 * The alpha/transparency value of the Colour.
		 */
		float a = 1;

		/**
		 * Creates a colour with the given rgba values.
		 */
		constexpr Colour(float const& r = 1, float const& g = 1, float const& b = 1, float const& a = 1) : r(r), g(g), b(b), a(a) { }
		
		static Colour white() { return { 1, 1, 1, 1 }; }
		static Colour black() { return { 0, 0, 0, 1 }; }
		
		static Colour red() { return { 1, 0, 0, 1 }; }
		static Colour green() { return { 0, 1, 0, 1 }; }
		static Colour blue() { return { 0, 0, 1, 1 }; }
		static Colour yellow() { return { 1, 1, 0, 1 }; }
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
		p.r = j["r"];
		p.g = j["g"];
		p.b = j["b"];
		p.a = j["a"];
	}

	static_assert(sizeof(Colour) == sizeof(float) * 4, "Colour can not be bigger than 4 floats!");
}
