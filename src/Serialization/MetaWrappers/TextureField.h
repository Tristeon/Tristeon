#pragma once
#include <Standard/String.h>
#include <Serialization/Type.h>

namespace Tristeon
{
	class Texture;
	
	struct TextureField
	{
		uint32_t guid;

		TextureField();
		explicit TextureField(const String& pPath);
		explicit TextureField(const uint32_t& pGuid);

		[[nodiscard]] String path() const;
		
		[[nodiscard]] Texture* get() const;
		[[nodiscard]] Texture* operator->() const;
		[[nodiscard]] Texture* operator*() const;

		[[nodiscard]] bool empty() const;
		[[nodiscard]] operator bool() const;
	};

	void to_json(nlohmann::json& j, const TextureField& p);
	void from_json(const nlohmann::json& j, TextureField& p);
}
