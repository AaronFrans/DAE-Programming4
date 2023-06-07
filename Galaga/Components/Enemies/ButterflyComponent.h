#pragma once
#include "BaseEnemyComponent.h"

namespace dae
{

	class ButterflyComponent final : public BaseEnemyComponent
	{

	public:
		void Attack();
		void OnHitCallback(const CollisionData& collisionOwner, const CollisionData& hitObject);

		ButterflyComponent(GameObject* owner);
		~ButterflyComponent() = default;

		ButterflyComponent(const ButterflyComponent& other) = delete;
		ButterflyComponent(ButterflyComponent&& other) = delete;
		ButterflyComponent& operator=(const ButterflyComponent& other) = delete;
		ButterflyComponent& operator=(ButterflyComponent&& other) = delete;

		void Update() override;

	private:
		enum class AttackStates
		{
			Idle,
			Zigzagging,
			Arcing,
			Returning
		};

		AttackStates m_CurAttackState{ AttackStates::Idle };

		void DoArcing(const float elapsed);
		void DoReturning(const float elapsed);
		void DoZigzagging(const float elapsed);



		static constexpr float MAX_ARC_TIME{ 1.5f };
		float m_CurArcTime{ 0 };

		static constexpr float MAX_X_OFFSET{ 30 };
		float m_CurOffset{};
		float m_OriginalXPos{};
		static constexpr float ZIGZAG_SPEED{ 300 };

		//-1 || 1
		int m_ZigzagOffsetSign{ 1 };

		glm::vec3 m_MovementDir{0, 100, 0};
	};

}
