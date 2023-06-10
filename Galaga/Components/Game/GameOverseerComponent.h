#pragma once
#include "Components/Component.h"

namespace dae
{
	struct Event;
	class DetailsComponent;
	class GameOverseerComponent final : public Component
	{
	public:
		GameOverseerComponent(GameObject* owner);


		~GameOverseerComponent();
		GameOverseerComponent(const GameOverseerComponent& other) = delete;
		GameOverseerComponent(GameOverseerComponent&& other) = delete;
		GameOverseerComponent& operator=(const GameOverseerComponent& other) = delete;
		GameOverseerComponent& operator=(GameOverseerComponent&& other) = delete;

		void SetSceneName(const std::string& name) { m_SceneName = name; };
		void SetNextSceneName(const std::string& name) { m_NextSceneName = name; };

		void SetDetails(DetailsComponent* details);

		void SkipLevel();

	private:

		void EnemyDead(const Event* e);
		void EnemyHit(const Event* e);
		void BulletShot(const Event* e);
		void EnemySpawned(const Event* e);

		int m_NrEnemies{ 0 };

		std::string m_SceneName{};
		std::string m_NextSceneName{};

		DetailsComponent* m_pDetails{};


		int m_NrShots{ 0 };
		int m_NrHits{ 0 };
	};

}
