#pragma once
#include "Components/Component.h"
#include "glm/glm.hpp"
#include <Scene/Scene.h>

namespace dae {


	struct CollisionData
	{
		std::string owner{ "" };
	};

	class TransformComponent;
	class CollisionComponent final : public Component
	{

	public:

		CollisionComponent(std::weak_ptr<GameObject> owner);

		~CollisionComponent();
		CollisionComponent(const CollisionComponent& other) = delete;
		CollisionComponent(CollisionComponent&& other) = delete;
		CollisionComponent& operator=(const CollisionComponent& other) = delete;
		CollisionComponent& operator=(CollisionComponent&& other) = delete;


		void SetBounds(float width, float height);

		CollisionData GetCollisionData() const;
		void SetCollisionData(CollisionData data);

		void SetScene(Scene* scene);

		void IsOverlappingOtherCollision(const std::vector<CollisionComponent*>& collisionsToCheck) const;


#if _DEBUG
		void Render() const override;
#endif

	private:

		TransformComponent* m_pOwnerTransform{};


		float m_Width{};
		float m_Height{};

		CollisionData m_CollisionData{};

		Scene* m_pScene{};

		bool CheckOverlapp(CollisionComponent* pOther) const;

	};
}

