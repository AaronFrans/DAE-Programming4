#pragma once
#include "Components/Component.h"
#include "Events/Event.h"

namespace dae
{
	class TextComponent;
	class LivesLeftComponent final : public Component
	{
	public:
		LivesLeftComponent(GameObject* owner);


		~LivesLeftComponent();
		LivesLeftComponent(const LivesLeftComponent& other) = delete;
		LivesLeftComponent(LivesLeftComponent&& other) = delete;
		LivesLeftComponent& operator=(const LivesLeftComponent& other) = delete;
		LivesLeftComponent& operator=(LivesLeftComponent&& other) = delete;

		void SetPlayerIndex(unsigned playerIndex);


		void Update() override;

	private:

		bool m_IsHit{false};
		float m_InvincibleTime{ 0 };

		void SetupRequiredComponents();
		void CheckForRequiredComponents();

		void SetLivesLeftText();

		void LoseLife(const Event* e);

		unsigned m_PlayerIndex{ 0 };

		int m_LivesLeft{ 3 };

		TextComponent* m_pTextComponent{ nullptr };
	};
}

