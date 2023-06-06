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
		~BeeComponent();

		BeeComponent(const BeeComponent& other) = delete;
		BeeComponent(BeeComponent&& other) = delete;
		BeeComponent& operator=(const BeeComponent& other) = delete;
		BeeComponent& operator=(BeeComponent&& other) = delete;

		void Update() override;

		void SetMaxYPos(float yPos);


	private:

		enum class AttackStates
		{
			Idle,
			Diving,
			Arcing,
			Returning
		};


		AttackStates m_CurAttackState{ AttackStates::Idle };

		void DoDiving(float elapsed);
		void DoArcing(float elapsed);
		void DoReturning(float elapsed);


		float m_CurArcTime{ 0 };

		glm::vec3 m_MovementDir{0, 100, 0};

		TransformComponent* m_pTransform{};



		float m_MaxYPos{ 600 };
	};

}

