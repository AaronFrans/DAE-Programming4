#pragma once
#include "Components/Component.h"
#include "Components/Collision/CollisionComponent.h"
#include "glm/glm.hpp"

namespace dae {

	class Texture2D;
	class TransformComponent;
	class SoundSystem;
	class Scene;
	struct Event;
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

		void SetPlayerIndex(unsigned playerIndex) { m_PlayerIndex = playerIndex; };


		void SetScene(const std::string& sceneName);

	private:

		void BulletHitCallback(const dae::CollisionData&, const dae::CollisionData& hitObject);

		void PlayerGrabbed(const Event* e);

		SoundSystem* m_Sound{};
		std::vector<std::shared_ptr<GameObject>> m_FiredBullets{};
		std::shared_ptr<Texture2D> m_AttackTexture{};

		std::string m_SceneName{};

		glm::vec2 m_BulletStartOffset{};
		TransformComponent* m_pPlayerTransform{};

		unsigned m_PlayerIndex{ 0 };
		bool m_CanPlayerShoot{ true };

		Scene* m_pScene{};
		bool m_HasDestroyedBullet{ false };


	};
}

