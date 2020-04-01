#pragma once

namespace Tristeon
{
	class Collider;
	
	class IContactEnd
	{
	public:
		virtual void contactEnd(Collider* other) = 0;
	};
}