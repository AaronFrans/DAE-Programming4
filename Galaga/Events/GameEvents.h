#pragma once
#include <Events/Event.h>

namespace dae
{
	struct PointEvent : public  Event
	{
		int NrPoints{};
	};
}