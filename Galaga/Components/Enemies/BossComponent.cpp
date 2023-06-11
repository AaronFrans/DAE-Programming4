#include "BossComponent.h"
#include <Engine/Timer.h>

#include <Components/TransformComponent.h>
#include <Components/ImageComponent.h>
#include <Components/Collision/CollisionComponent.h>
#include <Components/ImageRenderComponent.h>

#include "Commands/DiveCommand.h"
#include "Commands/TractorCommand.h"

#include "Rendering/ResourceManager.h"

#include <Events/EventManager.h>
#include "Events/GameEvents.h"

void dae::BossComponent::SetupCollision()
{
	m_TractorCollision->SetBounds(0, 0);
	m_pScene->AddCollision(m_TractorCollision);
	m_TractorCollision->SetScene(m_pScene);
	auto boundHitCallback = std::bind(&BossComponent::TractorBeamCallback, this, std::placeholders::_1, std::placeholders::_2);
	m_TractorCollision->SetCallback(boundHitCallback);
	m_TractorCollision->SetActive(false);
}

void dae::BossComponent::Update()
{
	if (!m_HasFlownIn)
		BaseEnemyComponent::Update();

	if (!m_IsAttacking)
		return;

	const auto elapsed = Timer::GetInstance().GetDeltaTime();
	if (m_HasGrabbedPlayer)
	{
		HandlePlayerCaught(elapsed);
		return;

	}

	switch (m_CurAttackState)
	{
	case dae::BossComponent::AttackStates::Idle:
		return;
		break;
	case dae::BossComponent::AttackStates::Diving:
		DoDiving(elapsed);
		break;
	case dae::BossComponent::AttackStates::Tractor:
		DoTractorBeam(elapsed);
		break;
	case dae::BossComponent::AttackStates::Arcing:
		DoArcing(elapsed);
		break;
	case dae::BossComponent::AttackStates::Returning:
		DoReturning(elapsed);
		break;
	}
}

void dae::BossComponent::OnButterflyDeath(const Event* e)
{
	if (strcmp(e->eventType, "ButterflyDied") != 0)
		return;

	if (const ButterflyDestroyedEvent* event = dynamic_cast<const ButterflyDestroyedEvent*>(e))
	{
		if (event->sceneName != m_SceneName)
			return;

		m_Butterflies.erase(std::remove(m_Butterflies.begin(), m_Butterflies.end(), event->butterfly), m_Butterflies.end());
	}

}

void dae::BossComponent::TractorBeamCallback(const CollisionData&, const CollisionData& hitObject)
{
	if (m_HasGrabbedPlayer)
		return;

	if (!(strcmp(hitObject.ownerType.c_str(), "Player") == 0))
		return;

	m_CaughtPlayerIndex = hitObject.playerIndex;

	std::unique_ptr<PlayerEvent> event = std::make_unique<PlayerEvent>();
	event->eventType = "PlayerGrabbed";
	event->playerIndex = hitObject.playerIndex;


	EventManager::GetInstance().SendEventMessage<PlayerEvent>(std::move(event));
	m_HasGrabbedPlayer = true;


	m_CaughtPlayer = hitObject.owningObject->GetTransform().get();

	m_PlayerOriginalPos = m_CaughtPlayer->GetWorldPosition();
	m_PlayerDragDir = m_pTransform->GetWorldPosition() - m_PlayerOriginalPos;
	m_PlayerDragDir = glm::normalize(m_PlayerDragDir);
	m_PlayerDragDir *= m_FlySpeed;


}

void dae::BossComponent::DoArcing(const float elapsed)
{
	auto curPos = m_pTransform->GetWorldPosition();

	curPos += m_MovementDir * elapsed;

	m_pTransform->SetLocalPosition(curPos);

	m_CurArcTime += elapsed;

	if (m_CurArcTime > MAX_ARC_TIME)
	{
		m_CurAttackState = AttackStates::Returning;

		m_MovementDir = (m_FormationPosition - curPos);

		m_MovementDir /= glm::length(m_MovementDir);
		m_MovementDir *= m_FlySpeed;
	}
}

void dae::BossComponent::DoReturning(const float elapsed)
{
	auto curPos = m_pTransform->GetWorldPosition();

	curPos += m_MovementDir * elapsed;

	m_pTransform->SetLocalPosition(curPos);

	if (glm::length(m_FormationPosition - curPos) > 1)
		return;


	m_CurAttackState = AttackStates::Idle;
	m_IsAttacking = false;

}

void dae::BossComponent::DoDiving(const float elapsed)
{
	auto curPos = m_pTransform->GetWorldPosition();

	curPos += m_MovementDir * elapsed;

	DoShooting(elapsed, curPos);

	m_pTransform->SetLocalPosition(curPos);

	if (curPos.y < m_MaxYPos)
		return;


	m_CurAttackState = AttackStates::Arcing;
	m_CurArcTime = 0;

	curPos.x > m_ScreenCenter.x ?
		m_MovementDir = { -m_FlySpeed, 0, 0 } :
		m_MovementDir = { m_FlySpeed, 0, 0 };
}

void dae::BossComponent::HandlePlayerCaught(const float elapsed)
{


	auto newPlayerPos = m_CaughtPlayer->GetWorldPosition();
	newPlayerPos += m_PlayerDragDir * elapsed;
	m_CaughtPlayer->SetLocalPosition(newPlayerPos);

	auto curPos = m_pTransform->GetWorldPosition();

	if (glm::length(curPos - newPlayerPos) > 1)
		return;

	m_HasGrabbedPlayer = false;

	m_CurAttackState = AttackStates::Returning;
	m_MovementDir = (m_FormationPosition - curPos);

	m_MovementDir /= glm::length(m_MovementDir);
	m_MovementDir *= m_FlySpeed;

	m_TractorBeamRender->SetActive(false);
	m_TractorCollision->SetActive(false);

	std::unique_ptr<PlayerEvent> event = std::make_unique<PlayerEvent>();
	event->eventType = "PlayerDied";
	event->playerIndex = m_CaughtPlayerIndex;
	EventManager::GetInstance().SendEventMessage(std::move(event));

	m_CaughtPlayer->SetLocalPosition(m_PlayerOriginalPos);
	m_CurTractorIndex = m_CaughtPlayerIndex;
	m_TractorBeamDisplay->SetTexture(m_TractorBeamTextures[m_CurTractorIndex]);
	auto textureWidth = m_TractorBeamDisplay->GetTextureWidth();
	auto textureHeight = m_TractorBeamDisplay->GetTextureHeight();
	m_TractorCollision->SetBounds(textureWidth, textureHeight);
}

void dae::BossComponent::DoTractorBeam(const float elapsed)
{

	auto curPos = m_pTransform->GetWorldPosition();

	if (curPos.y < m_MaxYPos / 2.0f)
	{
		DoTractorBeamDive(elapsed, curPos);
		return;
	}

	m_TractorBeamRender->SetActive(true);
	m_CurTractorStateChangeTime += elapsed;

	if (m_CurTractorStateChangeTime < MIN_TRACTOR_STATE_CHANGE_TIME)
		return;

	m_CurTractorStateChangeTime = 0;
	++m_CurTractorIndex;

	if (m_CurTractorIndex == static_cast<int>(m_TractorBeamTextures.size()))
	{


		m_CurAttackState = AttackStates::Returning;

		m_MovementDir = (m_FormationPosition - curPos);

		m_MovementDir /= glm::length(m_MovementDir);
		m_MovementDir *= m_FlySpeed;
		m_TractorBeamRender->SetActive(false);
		m_TractorCollision->SetActive(false);

		return;
	}


	m_TractorBeamDisplay->SetTexture(m_TractorBeamTextures[m_CurTractorIndex]);

	auto textureWidth = m_TractorBeamDisplay->GetTextureWidth();
	auto textureHeight = m_TractorBeamDisplay->GetTextureHeight();
	m_TractorCollision->SetBounds(textureWidth, textureHeight);

}

void dae::BossComponent::DoTractorBeamDive(const float elapsed, glm::vec3 curPos)
{

	curPos += m_MovementDir * elapsed;

	m_pTransform->SetLocalPosition(curPos);
}

void dae::BossComponent::Attack()
{

	bool doTracktorBeam = rand() % 2;
	if (doTracktorBeam)
	{
		DoTractorBeamAttack();
	}
	else
	{
		DoDivingAttack();
	}

}

void dae::BossComponent::DoDivingAttack()
{
	m_CurAttackState = AttackStates::Diving;

	m_IsAttacking = true;
	m_MovementDir = { 0, m_FlySpeed, 0 };

	std::vector<int> freeIndexes;

	for (int i = 0; i < static_cast<int>(m_Butterflies.size()); ++i) {
		if (!m_Butterflies[i]->IsAlreadyAttacking()) {
			freeIndexes.push_back(i);
		}
	}

	if (!freeIndexes.empty()) {
		// Generate a random index from the free indexes
		int randomIndex = freeIndexes[std::rand() % freeIndexes.size()];
		EnemyControllerComponent* randomButterfly = m_Butterflies[randomIndex];
		randomButterfly->ForceAttack();
	}
}

void dae::BossComponent::DoTractorBeamAttack()
{
	m_CurAttackState = AttackStates::Tractor;

	m_IsAttacking = true;
	m_MovementDir = { 0, m_FlySpeed, 0 };

	m_CurTractorIndex = 0;

	m_TractorCollision->SetActive(true);

	m_TractorBeamDisplay->SetTexture(m_TractorBeamTextures[m_CurTractorIndex]);
	auto textureWidth = m_TractorBeamDisplay->GetTextureWidth();
	auto textureHeight = m_TractorBeamDisplay->GetTextureHeight();
	m_TractorCollision->SetBounds(textureWidth, textureHeight);


	int randomIndex = std::rand() % m_pPlayerTransforms.size();

	m_MovementDir = m_pPlayerTransforms[randomIndex]->GetWorldPosition(); -m_pTransform->GetWorldPosition();

	m_MovementDir /= glm::length(m_MovementDir);
	m_MovementDir *= m_FlySpeed;
}

void dae::BossComponent::OnHitCallback(const CollisionData& collisionOwner, const CollisionData& hitObject)
{

	if (!(strcmp(hitObject.ownerType.c_str(), "PlayerAttack") == 0))
		return;

	if (!m_IsDamaged)
	{
		m_IsDamaged = true;
		m_DisplayComponent->SetTexture(m_DamagedTexture);
		return;
	}


	std::unique_ptr<SceneEvent> event = std::make_unique<SceneEvent>();
	event->eventType = "EnemyDied";
	event->sceneName = m_SceneName;

	switch (m_CurAttackState)
	{
	case dae::BossComponent::AttackStates::Idle:
		event->nrPoints = 150;
		break;
	case dae::BossComponent::AttackStates::Diving:
	case dae::BossComponent::AttackStates::Tractor:
	case dae::BossComponent::AttackStates::Arcing:
	case dae::BossComponent::AttackStates::Returning:
		event->nrPoints = 400;
		break;
	}

	EventManager::GetInstance().SendEventMessage(std::move(event));


	collisionOwner.owningObject->MarkForDestroy();

}

dae::BossComponent::BossComponent(GameObject* owner)
	:BaseEnemyComponent(owner)
{

	auto pTractor = std::make_shared<GameObject>();
	pTractor->Init();
	pTractor->SetParent(GetOwner(), false);

	m_TractorBeamDisplay = pTractor->AddComponent<ImageComponent>().get();
	m_TractorBeamRender = pTractor->AddComponent<ImageRenderComponent>().get();
	m_TractorBeamRender->SetActive(false);

	m_TractorBeamTextures.emplace_back(ResourceManager::GetInstance().LoadTexture("Images\\Galaga_Boss_Tractor_1.png"));
	m_TractorBeamTextures.emplace_back(ResourceManager::GetInstance().LoadTexture("Images\\Galaga_Boss_Tractor_2.png"));
	m_TractorBeamTextures.emplace_back(ResourceManager::GetInstance().LoadTexture("Images\\Galaga_Boss_Tractor_3.png"));
	m_TractorBeamTextures.emplace_back(ResourceManager::GetInstance().LoadTexture("Images\\Galaga_Boss_Tractor_4.png"));

	m_TractorBeamDisplay->SetTexture(m_TractorBeamTextures.back());
	auto textureWidth = m_TractorBeamDisplay->GetTextureWidth();

	pTractor->GetTransform()->SetLocalPosition({ -textureWidth / 4.0f, 30, 0 });
	m_DisplayComponent = GetOwner()->GetComponent<ImageComponent>().get();

	assert(m_DisplayComponent && "No ImageComponent set for the Boss GameObject");

	m_DamagedTexture = ResourceManager::GetInstance().LoadTexture("Images\\Galaga_Boss_Damaged.png");

	m_TractorCollision = pTractor->AddComponent<CollisionComponent>().get();

	m_TractorCollision->SetCollisionData({ "TractorBeam", pTractor.get() });


	auto boundButterflyDied = std::bind(&BossComponent::OnButterflyDeath, this, std::placeholders::_1);
	PlayerEvent event;
	event.eventType = "ButterflyDied";
	EventManager::GetInstance().AddObserver(event, boundButterflyDied);
}

dae::BossComponent::~BossComponent()
{

	auto boundButterflyDied = std::bind(&BossComponent::OnButterflyDeath, this, std::placeholders::_1);
	PlayerEvent event;
	event.eventType = "ButterflyDied";
	EventManager::GetInstance().RemoveObserver(event, boundButterflyDied);
}
