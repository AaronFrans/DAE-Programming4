#pragma once
#include "Components/Component.h"
#include "glm/glm.hpp"

namespace dae {

	class TransformComponent;
	class CollisionComponent final : public Component
	{

	public:

		CollisionComponent(std::weak_ptr<GameObject> owner);

		~CollisionComponent() = default;
		CollisionComponent(const CollisionComponent& other) = delete;
		CollisionComponent(CollisionComponent&& other) = delete;
		CollisionComponent& operator=(const CollisionComponent& other) = delete;
		CollisionComponent& operator=(CollisionComponent&& other) = delete;


		void SetBounds(float width, float height);

		void Render() const override {};

	private:

		TransformComponent* m_pOwnerTransform{};


		float m_Width{};
		float m_Height{};


	};
}

