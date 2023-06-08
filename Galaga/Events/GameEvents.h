#pragma once
#include <Events/Event.h>

namespace dae
{
	struct PointEvent : public  Event
	{
		int nrPoints{};
	};

	class EnemyControllerComponent;
	struct ButterflyDestroyedEvent : public  Event
	{
		EnemyControllerComponent* butterfly{};
	};
}