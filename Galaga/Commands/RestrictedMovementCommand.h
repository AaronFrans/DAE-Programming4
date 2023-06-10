#pragma once
#include "Commands/Command.h"
#include <glm/glm.hpp>

namespace dae {

	struct Event;
	class TransformComponent;
	class RestrictedMovementCommand final : public Command
	{
	public:
		RestrictedMovementCommand(GameObject* actor);
		~RestrictedMovementCommand();
		RestrictedMovementCommand(const RestrictedMovementCommand& other) = delete;
		RestrictedMovementCommand(RestrictedMovementCommand&& other) = delete;
		RestrictedMovementCommand& operator=(const RestrictedMovementCommand& other) = delete;
		RestrictedMovementCommand& operator=(RestrictedMovementCommand&& other) = delete;


		void Execute() override;

		void SetDirection(glm::vec3 direction) { m_Direction = direction; };
		void SetMinPoint(glm::vec3 minPoint) { m_MinPoint = minPoint; };
		void SetMaxPoint(glm::vec3 maxPoint) { m_MaxPoint = maxPoint; };
		void SetMoveSpeed(float moveSpeed) { m_MoveSpeed = moveSpeed; };

		void SetPlayerIndex(unsigned playerIndex) { m_PlayerIndex = playerIndex; };

	private:


		TransformComponent* m_pGameObjectTransform{ nullptr };


		unsigned m_PlayerIndex{ 0 };

		void PlayerGrabbed(const Event* e);
		void PlayerRespawn(const Event* e);

		bool m_CanPlayerMove{ true };

		glm::vec3 m_MinPoint{};
		glm::vec3 m_MaxPoint{};
		glm::vec3 m_Direction{};
		float m_MoveSpeed{};
	};
}

