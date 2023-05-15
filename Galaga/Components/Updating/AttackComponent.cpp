#include "AttackComponent.h"

#include "Rendering/Texture2D.h"
#include "Rendering/ResourceManager.h"

#include <Sounds/SoundManager.h>

#include "Components/TransformComponent.h"
#include "Components/ImageComponent.h"
#include "Components/ImageRenderComponent.h"

#include "Components/Updating/BulletMovementComponent.h"

dae::AttackComponent::AttackComponent(std::weak_ptr<GameObject> owner)
	:Component(owner)
{
	m_AttackTexture = ResourceManager::GetInstance().LoadTexture("Images\\Player_Attack.png");
	auto pOwner = owner.lock().get();

	m_pPlayerTransform = pOwner->GetTransform().get();

	glm::vec2 playerSpriteDimensions = pOwner->GetComponent<ImageComponent>()->GetTextureDimensions();
	glm::vec2 BulletSpriteDimensions = static_cast<glm::vec2>(m_AttackTexture->GetSize());

	m_BulletStartOffset = { playerSpriteDimensions.x / 2.0f + BulletSpriteDimensions.x / 2.0f, playerSpriteDimensions.y / 2.0f };
}

void dae::AttackComponent::Attack()
{

	if (m_FiredBullets.size() == 2)
		return;
	const glm::vec3 ownerPos = m_pPlayerTransform->GetWorldPosition();

	auto bullet = std::make_shared<GameObject>();
	bullet->Init();

	bullet->GetTransform()->SetLocalPosition({ ownerPos.x + m_BulletStartOffset.x,
		ownerPos.y + m_BulletStartOffset.y, ownerPos.z});

	bullet->AddComponent<ImageComponent>()->SetTexture(m_AttackTexture);
	bullet->AddComponent<BulletMovementComponent>();

	bullet->AddComponent<dae::ImageRenderComponent>();

	assert(m_pScene && "No scene set for AttackComponent, use AttackComponent::SetScene to set the scene");

	m_pScene->Add(bullet);

	m_FiredBullets.push_back(bullet);

	SoundManager::GetInstance().NotifySound(SoundData{ 1, 0.5, SoundData::SoundType::SoundEffect});

}

void dae::AttackComponent::Update()
{
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
