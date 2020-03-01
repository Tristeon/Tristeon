#pragma once

namespace Tristeon
{
	class Actor;
	
	class Behaviour
	{
	public:
		Behaviour() = default;
		virtual ~Behaviour() = default; //Enables V-Tables
		Behaviour(const Behaviour& other) = delete;
		Behaviour& operator=(const Behaviour& other) = delete;

		Behaviour(Behaviour&& other) noexcept;
		Behaviour& operator=(Behaviour&& other) noexcept;
		
	private:
		Actor* owner = nullptr;
	};
}
