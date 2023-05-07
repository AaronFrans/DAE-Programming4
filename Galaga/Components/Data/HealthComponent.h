#pragma once
#include "Components/Component.h"

namespace dae
{

	class HealthComponent final : public Component
	{
	public:
		HealthComponent(std::weak_ptr<GameObject> owner);

		~HealthComponent() = default;
		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		void TakeDamage(float damage);

		void SetPlayerIndex(unsigned playerIndex);

	private:

		float m_Health{ 20 };

		unsigned m_PlayerIndex{ 0 };

		void OutOfHealth();
	};


}