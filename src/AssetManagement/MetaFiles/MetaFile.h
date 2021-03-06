﻿#pragma once
#include "Serialization/Serializable.h"

#include <Standard/String.h>
#include <Standard/Unique.h>
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	/**
	 * \brief Default meta file when file extension cannot be recognized
	 */
	class MetaFile : public Serializable
	{
	public:
		virtual Unique<TObject> load(const String& path) { return nullptr; }

		json serialize() override;
		void deserialize(json j) override;
		
		String filepath;
		uint32_t GUID;
	};

	REGISTER_TYPE(MetaFile);
}
