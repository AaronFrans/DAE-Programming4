#pragma once
#include "Commands/Command.h"
#include <glm/glm.hpp>

namespace dae {
	class TransformComponent;
	class RestrictedMovementCommand final : public Command
	{
	public:
		RestrictedMovementCommand(GameObject* actor);

		void Execute() override;

		void SetDirection(glm::vec3 direction) { m_Direction = direction; };
		void SetMinPoint(glm::vec3 minPoint) { m_MinPoint = minPoint; };
		void SetMaxPoint(glm::vec3 maxPoint) { m_MaxPoint = maxPoint; };
		void SetMoveSpeed(float moveSpeed) { m_MoveSpeed = moveSpeed; };

	private:


		TransformComponent* m_pGameObjectTransform{ nullptr };



		glm::vec3 m_MinPoint{};
		glm::vec3 m_MaxPoint{};
		glm::vec3 m_Direction{};
		float m_MoveSpeed{};
	};
}

