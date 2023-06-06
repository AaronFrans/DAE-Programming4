#pragma once
#include "Components/Component.h"
#include "Components/Collision/CollisionComponent.h"
#include "glm/glm.hpp"

namespace dae {

	class Texture2D;
	class TransformComponent;
	class SoundSystem;
	class Scene;
	class AttackComponent final : public Component
	{
	public:
		AttackComponent(GameObject* owner);


		~AttackComponent() = default;
		AttackComponent(const AttackComponent& other) = delete;
		AttackComponent(AttackComponent&& other) = delete;
		AttackComponent& operator=(const AttackComponent& other) = delete;
		AttackComponent& operator=(AttackComponent&& other) = delete;


		void Attack();
		void Update();


		void SetScene(const std::string& sceneName);

	private:

		void BulletHitCallback(const dae::CollisionData& , const dae::CollisionData& hitObject);


		SoundSystem* m_Sound{};
		std::vector<std::shared_ptr<GameObject>> m_FiredBullets{};
		std::shared_ptr<Texture2D> m_AttackTexture{};

		std::string m_SceneName{};

		glm::vec2 m_BulletStartOffset{};
		TransformComponent* m_pPlayerTransform{};

		Scene* m_pScene{};
		bool m_HasDestroyedBullet{ false };


	};
}

