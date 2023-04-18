#pragma once
#include "Component.h"
#include "Event.h"

namespace dae
{
	class TextComponent;
	class LivesLeftComponent final : public Component
	{
	public:
		LivesLeftComponent(std::weak_ptr<GameObject> owner);


		~LivesLeftComponent() = default;
		LivesLeftComponent(const LivesLeftComponent& other) = delete;
		LivesLeftComponent(LivesLeftComponent&& other) = delete;
		LivesLeftComponent& operator=(const LivesLeftComponent& other) = delete;
		LivesLeftComponent& operator=(LivesLeftComponent&& other) = delete;

		void SetPlayerIndex(unsigned playerIndex);


	private:

		void SetupRequiredComponents();
		void CheckForRequiredComponents();

		void SetLivesLeftText();

		void LoseLife(const Event* e);

		unsigned m_PlayerIndex{ 0 };

		int m_LivesLeft{ 3 };

		TextComponent* m_pTextComponent{ nullptr };
	};
}

