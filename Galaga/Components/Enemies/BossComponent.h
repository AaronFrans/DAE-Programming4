#pragma once
#include "BaseEnemyComponent.h"

namespace dae
{
	class ImageComponent;
	class ImageRenderComponent;
	class CollisionComponent;
	class BossComponent final : public BaseEnemyComponent
	{
	public:
		void Attack();
		void OnHitCallback(const CollisionData& collisionOwner, const CollisionData& hitObject);

		BossComponent(GameObject* owner);
		~BossComponent() = default;

		BossComponent(const BossComponent& other) = delete;
		BossComponent(BossComponent&& other) = delete;
		BossComponent& operator=(const BossComponent& other) = delete;
		BossComponent& operator=(BossComponent&& other) = delete;

		void SetupCollision();
		void Update() override;

	private:
		enum class AttackStates
		{
			Idle,
			Diving,
			Tractor,
			Arcing,
			Returning
		};


		void TractorBeamCallback(const CollisionData& collisionOwner, const CollisionData& hitObject);

		AttackStates m_CurAttackState{ AttackStates::Idle };

		ImageComponent* m_TractorBeamDisplay{};
		ImageRenderComponent* m_TractorBeamRender{};
		CollisionComponent* m_TractorCollision{};

		void DoArcing(const float elapsed);
		void DoReturning(const float elapsed);
		void DoDiving(const float elapsed);


		void DoTractorBeam(const float elapsed);
		void DoTractorBeamDive(const float elapsed, glm::vec3 curPos);

		static constexpr float MAX_ARC_TIME{ 1.5f };
		float m_CurArcTime{ 0 };


		ImageComponent* m_DisplayComponent;
		std::shared_ptr<Texture2D> m_DamagedTexture{};
		bool m_IsDamaged{ false };

		glm::vec3 m_MovementDir{0, 100, 0};

		int m_CurTractorIndex{ 0 };
		static constexpr float MIN_TRACTOR_STATE_CHANGE_TIME{ 0.25f };
		float m_CurTractorStateChangeTime{ 0 };
		std::vector<std::shared_ptr<Texture2D>> m_TractorBeamTextures{};



		void HandlePlayerCaught(const float elapsed);
		TransformComponent* m_pPlayer{};

		glm::vec3 m_PlayerDragDir{};
	};
}

