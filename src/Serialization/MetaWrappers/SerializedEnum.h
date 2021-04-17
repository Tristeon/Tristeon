#pragma once
#include <json.h>
#include <magic_enum.hpp>
#include <Serialization/Type.h>

namespace Tristeon
{
	template<typename EnumType>
	struct SerializedEnum
	{
		EnumType value{};

		SerializedEnum() = default;
		explicit SerializedEnum(const EnumType& pValue) : value(pValue) { }
	};

	template<typename EnumType>
	inline void to_json(nlohmann::json& j, const SerializedEnum<EnumType>& p) {
#ifdef TRISTEON_EDITOR
		j["typeID"] = Type<SerializedEnum<EnumType>>::fullName();
#endif
		j["value"] = p.value;
	}

	template<typename EnumType>
	inline void from_json(const nlohmann::json& j, SerializedEnum<EnumType>& p) {
		if (j.type() == nlohmann::detail::value_t::number_unsigned)
			p.value = (EnumType)j.get<unsigned int>();
		else
			p.value = j.value("value", (EnumType)0);
	}
}

#ifdef TRISTEON_EDITOR
#include "Editor/Dynamic/EditorRegister.h"
#include "Editor/Dynamic/Fields/EnumEditor.h"

#define ENUM_EDITOR(enumType) EDITOR_(Tristeon::SerializedEnum<enumType>, TristeonEditor::EnumEditor<enumType>, enumType);
#else
#define ENUM_EDITOR(enumType)
#endif