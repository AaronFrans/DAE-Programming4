#pragma once
#include "Components/Component.h"
#include <glm/glm.hpp>
#include <Components/Collision/CollisionComponent.h>


namespace dae
{

	class BaseEnemyComponent : public Component
	{
	public:


		virtual void Attack() = 0;
		virtual void OnHitCallback(const CollisionData& collisionOwner, const CollisionData& hitObject) = 0;


		bool IsAttacking();

		void SetFormationPosition(glm::vec3 pos);


		~BaseEnemyComponent() = default;
		BaseEnemyComponent(const BaseEnemyComponent& other) = delete;
		BaseEnemyComponent(BaseEnemyComponent&& other) = delete;
		BaseEnemyComponent& operator=(const BaseEnemyComponent& other) = delete;
		BaseEnemyComponent& operator=(BaseEnemyComponent&& other) = delete;

	protected:

		BaseEnemyComponent(GameObject* owner);

		bool m_IsAttacking{ false };

		glm::vec3 m_FormationPosition{};
	private:

	};
}

