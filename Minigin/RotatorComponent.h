#pragma once
#include <numbers>
#include <glm/glm.hpp>
#include "Component.h"

namespace dae
{
	class TransformComponent;
	class RotatorComponent final :
		public Component
	{
	public:
		RotatorComponent(std::weak_ptr<GameObject> owner);

		~RotatorComponent() = default;
		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;
		RotatorComponent& operator=(const RotatorComponent& other) = delete;
		RotatorComponent& operator=(RotatorComponent&& other) = delete;

		void Update() override;

		void SetRotateSpeed(float speed);
		void SetCircleRadius(float radius);

		void SetPivot(glm::vec3 pivot);
	private:

		void CheckForRequiredComponents() const;
		void SetupRequiredComponents();
		std::weak_ptr<TransformComponent> m_Transform{};

		glm::vec3 m_Pivot{};


		float m_RotateSpeed{ 2 };
		float m_CircleRadius{ 20 };

		float m_LastCircleProgress{ 0 };
		const float MAX_ROTATION{ static_cast<float>(2.0f * std::numbers::pi)};


	};
}

