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
		std::string sceneName{};
		EnemyControllerComponent* butterfly{};
	};

	struct SceneEvent : public  Event
	{
		std::string sceneName{};
		int nrPoints{};
	};
}