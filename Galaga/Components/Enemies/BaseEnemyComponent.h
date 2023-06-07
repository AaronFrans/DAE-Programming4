#pragma once
#include "Components/Component.h"
#include <glm/glm.hpp>


namespace dae
{
	class Texture2D;
	class SoundSystem;
	class Scene;
	struct CollisionData;
	class BaseEnemyComponent : public Component
	{
	public:


		virtual void Attack() = 0;
		virtual void OnHitCallback(const CollisionData& collisionOwner, const CollisionData& hitObject) = 0;


		bool IsAttacking();

		void SetFormationPosition(glm::vec3 pos);
		void SetPlayerTransform(const TransformComponent* player);
		void SetScreenCenter(const glm::vec3 center);
		virtual void SetScene(const std::string& sceneName);
		void SetMaxYPos(const float maxYPos);


		virtual ~BaseEnemyComponent() = default;
		BaseEnemyComponent(const BaseEnemyComponent& other) = delete;
		BaseEnemyComponent(BaseEnemyComponent&& other) = delete;
		BaseEnemyComponent& operator=(const BaseEnemyComponent& other) = delete;
		BaseEnemyComponent& operator=(BaseEnemyComponent&& other) = delete;

	protected:

		void BulletHitCallback(const dae::CollisionData&, const dae::CollisionData& hitObject);


		void DoShooting(const float elapsed, const glm::vec3 curPos);

		static constexpr float MIN_FIRE_COOLDOWN{ 0.5f };

		float m_CurFireCooldownTime{ 0 };

		BaseEnemyComponent(GameObject* owner);

		glm::vec3 m_FormationPosition{};
		glm::vec3 m_ScreenCenter{};

		const TransformComponent* m_pPlayerTransform{};
		TransformComponent* m_pTransform{};

		bool m_IsAttacking{ false };

		//Shooting DataMembers
		std::shared_ptr<Texture2D> m_AttackTexture{};
		SoundSystem* m_Sound{};

		Scene* m_pScene{};
		std::string m_SceneName{};

		//Constants
		float m_MaxYPos{ 600 };

	};
}

