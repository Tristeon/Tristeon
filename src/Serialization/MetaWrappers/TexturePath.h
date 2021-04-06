#pragma once
#include <Serialization/Serializable.h>
#include <Standard/String.h>

#include <Rendering/Texture.h>
#include <Serialization/Type.h>

namespace Tristeon
{
	struct TexturePath
	{
		String path;

		TexturePath() = default;
		explicit TexturePath(const String& pPath) : path(pPath) { }
	};

	inline void to_json(nlohmann::json& j, const TexturePath& p) {
#ifdef TRISTEON_EDITOR
		j["typeID"] = Type<TexturePath>::fullName();
#endif
		j["path"] = p.path;
	}
	
	inline void from_json(const nlohmann::json& j, TexturePath& p) {
		p.path = j.value("path", Texture::defaultPath);
	}
}