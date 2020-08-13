#pragma once
#include "TypeDefinitions.h"
#include "Serialization/Serializable.h"

namespace Tristeon
{
	class MetaFile : public Serializable
	{
	public:
		virtual Unique<TObject> load() = 0;

		json serialize() override;

		void deserialize(json j) override;
		
		String filepath;
		
		int GUID;
	};
}
