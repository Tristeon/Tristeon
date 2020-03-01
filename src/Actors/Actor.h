#pragma once
#include "Behaviours/Behaviour.h"
#include "Behaviours/SpriteBehaviour.h"
#include "Math/Vector2Int.h"

namespace Tristeon
{
	class ActorLayer;
	class SceneManager;

	template<typename T>
	using T_is_behaviour = std::enable_if_t<std::is_base_of<Behaviour, T>::value, T>;
	
	class Actor
	{
		friend ActorLayer;
		friend SceneManager;
	public:
		Vector2 position = { 0, 0 };
		Vector2 scale = { 0, 0 };
		float rotation = 0;

		template<typename T>
		T_is_behaviour<T>* behaviour();

		template<typename T>
		T_is_behaviour<T>* addBehaviour();

		//TODO: Cache known behaviours
		SpriteBehaviour* sprite() { return behaviour<SpriteBehaviour>(); }

	private:
		std::vector<std::unique_ptr<Behaviour>> behaviours;
	};

	template <typename T>
	T_is_behaviour<T>* Actor::behaviour()
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
	T_is_behaviour<T>* Actor::addBehaviour()
	{
		T* result = new T();
		result->_owner = this;
		behaviours.push_back(std::unique_ptr<T>(result));
		return result;
	}
}
