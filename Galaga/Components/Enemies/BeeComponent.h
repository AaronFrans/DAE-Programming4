#pragma once
#include "BaseEnemyComponent.h"

namespace dae
{
	class BeeComponent final : public BaseEnemyComponent
	{

	public:

		void Attack();
		void OnHitCallback(const CollisionData& collisionOwner, const CollisionData& hitObject);

		BeeComponent(GameObject* owner);
		~BeeComponent() = default;

		BeeComponent(const BeeComponent& other) = delete;
		BeeComponent(BeeComponent&& other) = delete;
		BeeComponent& operator=(const BeeComponent& other) = delete;
		BeeComponent& operator=(BeeComponent&& other) = delete;

		void Update() override;


	private:

		enum class AttackStates
		{
			Idle,
			Diving,
			Arcing,
			Returning
		};


		AttackStates m_CurAttackState{ AttackStates::Idle };

		void DoDiving(const float elapsed);
		void DoArcing(const float elapsed);
		void DoReturning(const float elapsed);



		static constexpr float MAX_ARC_TIME{ 1 };
		float m_CurArcTime{ 0 };

		glm::vec3 m_MovementDir{0, 100, 0};
	};

}

