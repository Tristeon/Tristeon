#include "TextureField.h"

#include <AssetManagement/AssetDatabase.h>
#include <AssetManagement/Resources.h>

#include <Rendering/Texture.h>

namespace Tristeon
{
	TextureField::TextureField() : guid(0)
	{
		
	}

	TextureField::TextureField(const String& pPath) : guid(AssetDatabase::guid(pPath))
	{
		
	}

	TextureField::TextureField(const uint32_t& pGuid) : guid(pGuid)
	{
		
	}

	String TextureField::path() const
	{
		return AssetDatabase::path(guid);
	}

	Texture* TextureField::get() const
	{
		auto* tex = Resources::load<Texture>(guid);
		if (!tex)
			tex = Resources::load<Texture>(Texture::defaultPath);

		return tex;
	}

	bool TextureField::empty() const
	{
		return guid == 0 || AssetDatabase::path(guid).empty();
	}

	TextureField::operator bool() const
	{
		return !empty();
	}

	Texture* TextureField::operator->() const
	{
		return get();
	}

	Texture* TextureField::operator*() const
	{
		return get();
	}

	void to_json(json& j, const TextureField& p)
	{
#ifdef TRISTEON_EDITOR
		j["typeID"] = Type<TextureField>::fullName();
#endif
		j["guid"] = p.guid;
	}

	void from_json(const json& j, TextureField& p)
	{
		if (j.contains("path"))
		{
			const std::string path = j["path"];
			p.guid = AssetDatabase::guid(path);
		}
		else if (j.contains("guid"))
			p.guid = j["guid"];
	}
}
