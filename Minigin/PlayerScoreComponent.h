#pragma once
#include "Component.h"
#include "Event.h"

namespace dae
{
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


	private:

		const static int POINT_FOR_ENEMY{ 100 };


		void SetupRequiredComponents();
		void CheckForRequiredComponents();

		void SetPointsText();

		void EarnPoints(const Event* e);

		unsigned m_PlayerIndex{ 0 };

		int m_PointsEarned{ 0 };

		TextComponent* m_pTextComponent{ nullptr };
	};
}