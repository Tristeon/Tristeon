#pragma once

namespace Tristeon
{
	class Collider;
	
	class IContactBegin
	{
	public:
		virtual void contactBegin(Collider* other) = 0;
	};
}