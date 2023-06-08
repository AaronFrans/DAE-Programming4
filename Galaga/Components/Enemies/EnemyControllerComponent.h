#pragma once
#include "Components/Component.h"

namespace dae
{
	class BaseEnemyComponent;
	class EnemyControllerComponent final : public Component
	{
	public:
		EnemyControllerComponent(GameObject* owner);

		~EnemyControllerComponent() = default;
		EnemyControllerComponent(const EnemyControllerComponent& other) = delete;
		EnemyControllerComponent(EnemyControllerComponent&& other) = delete;
		EnemyControllerComponent& operator=(const EnemyControllerComponent& other) = delete;
		EnemyControllerComponent& operator=(EnemyControllerComponent&& other) = delete;

		void Update() override;

		void AddMinDelayOffset(float offset) { m_MinAttackDelay += offset; };

		void ForceAttack();
		bool IsAlreadyAttacking();

	private:
		BaseEnemyComponent* m_pControlledEnemy{};

		float m_MinAttackDelay{ 2.f };
		float m_CurAttackDelay{ 0 };
	};
}

