#pragma once
#include "Component.h"
#include "Event.h"
#include <memory>

namespace dae
{
	class SteamAchievements;
	class TextComponent;
	class PlayerScoreComponent final : public Component
	{
	public:
		PlayerScoreComponent(std::weak_ptr<GameObject> owner);


		~PlayerScoreComponent() = default;
		PlayerScoreComponent(const PlayerScoreComponent& other) = delete;
		PlayerScoreComponent(PlayerScoreComponent&& other) = delete;
		PlayerScoreComponent& operator=(const PlayerScoreComponent& other) = delete;
		PlayerScoreComponent& operator=(PlayerScoreComponent&& other) = delete;

		void SetPlayerIndex(unsigned playerIndex);
		void LinkAchievements(const std::shared_ptr<SteamAchievements>& gameAchievements);


	private:

		const static int POINT_FOR_ENEMY{ 100 };
		const static int ACH_WIN_ONE_GAME_MIN_POINTS{ 500 };


		void SetupRequiredComponents();
		void CheckForRequiredComponents();

		void SetPointsText();

		void EarnPoints(const Event* e);

		void CheckForAchievements();

		unsigned m_PlayerIndex{ 0 };

		int m_PointsEarned{ 0 };

		std::shared_ptr<SteamAchievements> m_GameAchievements;


		TextComponent* m_pTextComponent{ nullptr };
	};
}