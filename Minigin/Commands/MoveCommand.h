#pragma once
#include "Command.h"
#include <glm/glm.hpp>

namespace dae
{
	class TransformComponent;
	class MoveCommand final : public Command
	{
	public:
		MoveCommand(GameObject* actor);

		void Execute() override;

		void SetDirection(glm::vec3 direction) { m_Direction = direction; };
		void SetMoveSpeed(float moveSpeed) { m_MoveSpeed = moveSpeed; };

	private:

		//don't delete -> not owned
		GameObject* m_pActor{ nullptr };


		TransformComponent* m_pGameObjectTransform{ nullptr };

		glm::vec3 m_Direction{};
		float m_MoveSpeed{};
	};
}

