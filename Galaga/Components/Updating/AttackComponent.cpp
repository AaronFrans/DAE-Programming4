#include "AttackComponent.h"

#include "Engine/GameObject.h"

#include "Rendering/Texture2D.h"
#include "Rendering/ResourceManager.h"

#include <Sounds/SoundManager.h>
#include <Sounds/SoundSystem.h>

#include "Events/EventManager.h"

#include "Engine/Timer.h"

#include "Components/TransformComponent.h"
#include "Components/ImageComponent.h"
#include "Components/ImageRenderComponent.h"

#include "Components/Updating/BulletMovementComponent.h"
#include <Events/GameEvents.h>

dae::AttackComponent::AttackComponent(GameObject* owner)
	:Component(owner)
{
	m_AttackTexture = ResourceManager::GetInstance().LoadTexture("Images\\Player_Attack.png");
	auto pOwner = owner;

	m_pPlayerTransform = pOwner->GetTransform().get();

	glm::vec2 playerSpriteDimensions = pOwner->GetComponent<ImageComponent>()->GetTextureDimensions();
	glm::vec2 BulletSpriteDimensions = static_cast<glm::vec2>(m_AttackTexture->GetSize());

	m_BulletStartOffset = { playerSpriteDimensions.x / 2.0f + BulletSpriteDimensions.x / 2.0f, playerSpriteDimensions.y / 2.0f };
	m_Sound = SoundManager::GetInstance().GetSoundSystem();

	auto boundPlayerGrabbed = std::bind(&AttackComponent::PlayerGrabbed, this, std::placeholders::_1);
	PlayerEvent event;
	event.eventType = "PlayerGrabbed";
	EventManager::GetInstance().AddObserver(event, boundPlayerGrabbed);

	auto boundPlayerRespawn = std::bind(&AttackComponent::PlayerRespawn, this, std::placeholders::_1);
	PlayerEvent respawnEvent;
	respawnEvent.eventType = "PlayerRespawn";
	EventManager::GetInstance().AddObserver(respawnEvent, boundPlayerRespawn);

}

dae::AttackComponent::~AttackComponent()
{
	auto boundPlayerGrabbed = std::bind(&AttackComponent::PlayerGrabbed, this, std::placeholders::_1);
	PlayerEvent event;
	event.eventType = "PlayerGrabbed";
	EventManager::GetInstance().RemoveObserver(event, boundPlayerGrabbed);

	auto boundPlayerRespawn = std::bind(&AttackComponent::PlayerRespawn, this, std::placeholders::_1);
	PlayerEvent respawnEvent;
	respawnEvent.eventType = "PlayerRespawn";
	EventManager::GetInstance().RemoveObserver(respawnEvent, boundPlayerRespawn);
}

void dae::AttackComponent::Attack()
{
	if (!m_IsIntroDone)
		return;

	if (!m_CanPlayerShoot)
		return;


	assert(m_pScene && "No scene set for AttackComponent, use AttackComponent::SetScene to set the scene");

	if (m_FiredBullets.size() == 2)
		return;

	const glm::vec3 ownerPos = m_pPlayerTransform->GetWorldPosition();

	auto bullet = std::make_shared<GameObject>();
	bullet->Init();

	bullet->GetTransform()->SetLocalPosition(
		{ ownerPos.x + m_BulletStartOffset.x,
		ownerPos.y + m_BulletStartOffset.y, ownerPos.z }
	);

	bullet->AddComponent<ImageComponent>()->SetTexture(m_AttackTexture);
	bullet->AddComponent<BulletMovementComponent>()->SetMoveSpeed(500);

	bullet->AddComponent<dae::ImageRenderComponent>();

	auto collision = bullet->AddComponent<dae::CollisionComponent>();
	collision->SetCollisionData({ "PlayerAttack", bullet.get() });

	float collisionWidth{ 11 }, collisionHeight{ 22 };
	collision->SetBounds(collisionWidth, collisionHeight);


	auto boundHitCallback = std::bind(&AttackComponent::BulletHitCallback, this, std::placeholders::_1, std::placeholders::_2);
	collision->SetCallback(boundHitCallback);

	collision->SetScene(m_pScene);

	m_pScene->Add(bullet);

	m_pScene->AddCollision(collision.get());

	m_FiredBullets.push_back(bullet);

	std::unique_ptr<SceneEvent> hitEvent = std::make_unique<SceneEvent>();
	hitEvent->eventType = "BulletFired";
	hitEvent->sceneName = m_SceneName;
	EventManager::GetInstance().SendEventMessage(std::move(hitEvent));


	//TODO: find a better way to handle sound ids
	m_Sound->HandleSoundData(SoundData{ 1, 0.1f, SoundData::SoundType::SoundEffect });

}

void dae::AttackComponent::Update()
{


	if (!m_IsIntroDone)
	{
		// TODO: change to event
		constexpr int introTime{ 1 };

		static float timeSinceStart{ 0 };

		timeSinceStart += Timer::GetInstance().GetDeltaTime();

		if (timeSinceStart >= introTime)
		{
			m_IsIntroDone = true;
		}
	}

	for (auto& bullet : m_FiredBullets)
	{
		if (bullet->IsDestroyed())
		{
			m_HasDestroyedBullet = true;
		}
	}

	if (m_HasDestroyedBullet)
	{
		m_FiredBullets.erase(std::remove_if(begin(m_FiredBullets), end(m_FiredBullets),
			[&](std::shared_ptr<GameObject> bullet) { return bullet->IsDestroyed(); }),
			end(m_FiredBullets));

		m_HasDestroyedBullet = false;
	}
}

void dae::AttackComponent::SetScene(const std::string& sceneName)
{
	m_SceneName = sceneName;

	m_pScene = &SceneManager::GetInstance().GetSceneByName(m_SceneName);
}

void dae::AttackComponent::BulletHitCallback(const dae::CollisionData& collisionOwner, const dae::CollisionData& hitObject)
{
	if (!(strcmp(hitObject.ownerType.c_str(), "Enemy") == 0))
		return;


	std::unique_ptr<SceneEvent> hitEvent = std::make_unique<SceneEvent>();
	hitEvent->eventType = "EnemyHit";
	hitEvent->sceneName = m_SceneName;
	EventManager::GetInstance().SendEventMessage(std::move(hitEvent));

	SoundManager::GetInstance().GetSoundSystem()->HandleSoundData(dae::SoundData{ 3, 0.1f, });

	collisionOwner.owningObject->MarkForDestroy();
}


void dae::AttackComponent::PlayerGrabbed(const Event* e)
{
	if (strcmp(e->eventType, "PlayerGrabbed") != 0)
		return;

	if (const PlayerEvent* event = dynamic_cast<const PlayerEvent*>(e))
	{
		if (event->playerIndex != m_PlayerIndex)
			return;

		m_CanPlayerShoot = false;
	}

}

void dae::AttackComponent::PlayerRespawn(const Event* e)
{


	if (strcmp(e->eventType, "PlayerRespawn") != 0)
		return;

	if (const PlayerEvent* event = dynamic_cast<const PlayerEvent*>(e))
	{
		if (event->playerIndex != m_PlayerIndex)
			return;

		m_CanPlayerShoot = true;
	}

}
