#pragma once
#include "Components/Component.h"

namespace dae {

	class TransformComponent;

	class BulletMovementComponent final : public Component
	{

	public:
		BulletMovementComponent(GameObject* owner);

		~BulletMovementComponent() = default;
		BulletMovementComponent(const BulletMovementComponent& other) = delete;
		BulletMovementComponent(BulletMovementComponent&& other) = delete;
		BulletMovementComponent& operator=(const BulletMovementComponent& other) = delete;
		BulletMovementComponent& operator=(BulletMovementComponent&& other) = delete;


		void Update() override;

		void SetMoveSpeed(float moveSpeed);
		void SetMaxDistance(float maxDistance);

	private:

		float m_MoveSpeed{ 600 };

		float m_MaxDistance{ 500 };
		float m_DistanceTraveled{ 0 };

		TransformComponent* m_pBulletTransform{};
	};


}

