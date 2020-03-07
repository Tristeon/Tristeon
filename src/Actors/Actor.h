#pragma once
#include "Behaviours/Behaviour.h"
#include "Behaviours/Sprite.h"
#include "Math/Vector2Int.h"
#include "TypeDefinitions.h"

#include <Serialization/Serializable.h>
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	class ActorLayer;
	class SceneManager;

	template<typename T>
	using IsBehaviour = std::enable_if_t<std::is_base_of<Behaviour, T>::value, T>;
	
	class Actor : public Serializable
	{
		REGISTER_TYPE_H(Actor)
		
		friend ActorLayer;
		friend SceneManager;
	public:
		Vector2 position = { 0, 0 };
		Vector2 scale = { 0, 0 };
		float rotation = 0;
		std::string name = "";
		
		json serialize() override;
		void deserialize(json j) override;

		std::string getTag() const { return tag; }
		
		template<typename T>
		IsBehaviour<T>* behaviour();

		template<typename T>
		IsBehaviour<T>* addBehaviour();

		//TODO: Cache known behaviours
		Sprite* sprite() { return behaviour<Sprite>(); }

	private:
		Vector<Unique<Behaviour>> behaviours;
		std::string tag = "";
	};

	template <typename T>
	IsBehaviour<T>* Actor::behaviour()
	{
		for (auto const& behaviour : behaviours)
		{
			T* result = dynamic_cast<T*>(behaviour.get());
			if (result != nullptr)
				return result;
		}
		return nullptr;
	}

	template <typename T>
	IsBehaviour<T>* Actor::addBehaviour()
	{
		T* result = new T();
		result->_owner = this;
		behaviours.push_back(Unique<T>(result));
		return result;
	}
}
