#pragma once
#include "Components/Component.h"

namespace dae
{
	struct Event;
	class GameOverseerComponent final : public Component
	{
	public:
		GameOverseerComponent(GameObject* owner);


		~GameOverseerComponent() = default;
		GameOverseerComponent(const GameOverseerComponent& other) = delete;
		GameOverseerComponent(GameOverseerComponent&& other) = delete;
		GameOverseerComponent& operator=(const GameOverseerComponent& other) = delete;
		GameOverseerComponent& operator=(GameOverseerComponent&& other) = delete;

	private:

		void EnemyDead(const Event* e);
		void EnemySpawned(const Event* e);

		int m_NrEnemies{ 0 };
	};

}
