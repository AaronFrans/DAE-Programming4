#pragma once
#include "Components/Component.h"
#include <functional>
#include "glm/glm.hpp"
#include <Scene/Scene.h>

namespace dae {


	struct CollisionData
	{
		std::string ownerType{ "" };
		GameObject* owningObject{};
	};



	class TransformComponent;
	class CollisionComponent final : public Component
	{

	public:

		CollisionComponent(GameObject* owner);

		~CollisionComponent();
		CollisionComponent(const CollisionComponent& other) = delete;
		CollisionComponent(CollisionComponent&& other) = delete;
		CollisionComponent& operator=(const CollisionComponent& other) = delete;
		CollisionComponent& operator=(CollisionComponent&& other) = delete;


		void SetBounds(float width, float height);

		CollisionData GetCollisionData() const;
		void SetCollisionData(CollisionData data);

		void SetCallback(std::function<void(const CollisionData& owner, const CollisionData& hitObject)> callbackFunc);


		void SetScene(Scene* scene);

		void IsOverlappingOtherCollision(const std::vector<CollisionComponent*>& collisionsToCheck) const;


#if _DEBUG
		void Render() const override;
		void EnableDebugSquare() { m_DrawDebugLines = true; };
#endif

	private:

		TransformComponent* m_pOwnerTransform{};

#if _DEBUG
		bool m_DrawDebugLines{ false };
#endif
		float m_Width{};
		float m_Height{};

		CollisionData m_CollisionData{};

		Scene* m_pScene{};

		bool CheckOverlapp(CollisionComponent* pOther) const;


		std::function<void(const CollisionData& collisionOwner, const CollisionData& hitObject)> m_OnHitCallback{};

	};
}

