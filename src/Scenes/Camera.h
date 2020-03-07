#pragma once
#include <Math/Vector2Int.h>
#include <Serialization/Serializable.h>
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	class Camera : public Serializable
	{
		REGISTER_TYPE_H(Camera)
	public:
		Vector2Int position;
		Vector2Int size;
		float zoom;
		
		json serialize() override;
		void deserialize(json j) override;
	};
}