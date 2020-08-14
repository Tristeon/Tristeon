#pragma once
#include "TypeDefinitions.h"
#include "Serialization/Serializable.h"

namespace Tristeon
{
	/**
	 * \brief Default meta file when file extension cannot be recognized
	 */
	class MetaFile : public Serializable
	{
	public:
		virtual Unique<TObject> load() { return nullptr; }

		json serialize() override;

		void deserialize(json j) override;
		
		String filepath;
		
		unsigned int GUID;
	};
}
