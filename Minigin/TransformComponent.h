#pragma once
#include <glm/glm.hpp>
#include "Component.h"
namespace dae
{

	class TransformComponent final : public Component
	{
	public:
		TransformComponent(std::weak_ptr<GameObject> owner);

		~TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;

		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetLocalPosition() const;
		void SetLocalPosition(const glm::vec3& newPos);

		void SetPositionDirty();
	private:
		void UpdateWorldPosition();

		bool m_IsDirty{ false };
		glm::vec3 m_WorldPostition{0, 0, 0};
		glm::vec3 m_LocalPostition{ 0, 0, 0 };
	};
}
