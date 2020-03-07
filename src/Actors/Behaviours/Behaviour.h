#pragma once
#include <Serialization/Serializable.h>

namespace Tristeon
{
	class Actor;
	class ActorLayer;
	
	class Behaviour : public Serializable
	{
		friend Actor;
		friend ActorLayer;
	public:
		Behaviour();
		virtual ~Behaviour();
		Behaviour(const Behaviour& other) = delete;
		Behaviour& operator=(const Behaviour& other) = delete;

		json serialize() override = 0;
		void deserialize(json j) override = 0;

		Behaviour(Behaviour&& other) noexcept;
		Behaviour& operator=(Behaviour&& other) noexcept;

		Actor* owner() const { return _owner; }
	private:
		virtual void update() { /* Empty */ }
		Actor* _owner = nullptr;
	};
}
