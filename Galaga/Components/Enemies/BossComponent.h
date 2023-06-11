#pragma once
#pragma once
#include "BaseEnemyComponent.h"
#include "Components/Enemies/EnemyControllerComponent.h"

namespace dae
{
	class ImageComponent;
	class ImageRenderComponent;
	class CollisionComponent;
	class EnemyControllerComponent;
	class TractorCommand;
	class DiveCommand;
	struct Event;
	class BossComponent final : public BaseEnemyComponent
	{
	public:
		void Attack();
		void OnHitCallback(const CollisionData& collisionOwner, const CollisionData& hitObject);

		BossComponent(GameObject* owner);
		~BossComponent();

		BossComponent(const BossComponent& other) = delete;
		BossComponent(BossComponent&& other) = delete;
		BossComponent& operator=(const BossComponent& other) = delete;
		BossComponent& operator=(BossComponent&& other) = delete;

		void SetButterflies(std::vector<EnemyControllerComponent*>& butterflies) { m_Butterflies = butterflies; };

		void SetDamagedTexture(std::shared_ptr<Texture2D> texture) { m_DamagedTexture = texture; };

		void SetupCollision();
		void Update() override;



	private:

		friend class DiveCommand;
		friend class TractorCommand;

		enum class AttackStates
		{
			Idle,
			Diving,
			Tractor,
			Arcing,
			Returning
		};


		void DoDivingAttack();
		void DoTractorBeamAttack();

		void OnButterflyDeath(const Event* e);

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





		std::vector<EnemyControllerComponent*> m_Butterflies{};
		void HandlePlayerCaught(const float elapsed);
		bool m_HasGrabbedPlayer{ false };
		TransformComponent* m_CaughtPlayer{};
		int m_CaughtPlayerIndex{ 0 };
		glm::vec3 m_PlayerOriginalPos{};
		glm::vec3 m_PlayerDragDir{};
	};
}

