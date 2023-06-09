#pragma once
#include "Components/Component.h"
#include "Events/Event.h"
#include <memory>

namespace dae
{
	class TextComponent;
	class PlayerScoreComponent final : public Component
	{
	public:
		PlayerScoreComponent(GameObject* owner);


		~PlayerScoreComponent() = default;
		PlayerScoreComponent(const PlayerScoreComponent& other) = delete;
		PlayerScoreComponent(PlayerScoreComponent&& other) = delete;
		PlayerScoreComponent& operator=(const PlayerScoreComponent& other) = delete;
		PlayerScoreComponent& operator=(PlayerScoreComponent&& other) = delete;

		void SetPlayerIndex(unsigned playerIndex);


	private:

		const static int POINT_FOR_ENEMY{ 100 };
		const static int ACH_WIN_ONE_GAME_MIN_POINTS{ 500 };


		void SetupRequiredComponents();
		void CheckForRequiredComponents();

		void SetPointsText();

		void EarnPoints(const Event* e);

		unsigned m_PlayerIndex{ 0 };

		int m_PointsEarned{ 0 };

		void FinalScore(const Event* e);

		TextComponent* m_pTextComponent{ nullptr };
	};
}