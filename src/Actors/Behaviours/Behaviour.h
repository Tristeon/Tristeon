#pragma once

namespace Tristeon
{
	class Actor;
	class ActorLayer;
	
	class Behaviour
	{
		friend Actor;
		friend ActorLayer;
	public:
		Behaviour() = default;
		virtual ~Behaviour() = default; //Enables V-Tables
		Behaviour(const Behaviour& other) = delete;
		Behaviour& operator=(const Behaviour& other) = delete;

		Behaviour(Behaviour&& other) noexcept;
		Behaviour& operator=(Behaviour&& other) noexcept;

		Actor* owner() const { return _owner; }
	private:
		virtual void update() { /* Empty */ }
		Actor* _owner = nullptr;
	};
}
