#include "LivesLeftComponent.h"
#include "Events/EventManager.h"
#include "Components/TextComponent.h"

#include <Engine/Timer.h>
#include <memory>
#include <Events/GameEvents.h>
#include <Scene/Scene.h>
#include <Scene/SceneManager.h>

dae::LivesLeftComponent::LivesLeftComponent(GameObject* owner)
	:Component(owner)
{
	SetupRequiredComponents();
	CheckForRequiredComponents();

	SetLivesLeftText();

	auto boundLoseLife = std::bind(&LivesLeftComponent::LoseLife, this, std::placeholders::_1);
	PlayerEvent event;
	event.eventType = "PlayerDied";
	EventManager::GetInstance().AddObserver(event, boundLoseLife);
}

dae::LivesLeftComponent::~LivesLeftComponent()
{
	auto boundLoseLife = std::bind(&LivesLeftComponent::LoseLife, this, std::placeholders::_1);
	PlayerEvent event;
	event.eventType = "PlayerDied";
	EventManager::GetInstance().RemoveObserver(event, boundLoseLife);
}

void dae::LivesLeftComponent::SetPlayerIndex(unsigned playerIndex)
{
	m_PlayerIndex = playerIndex;
}

void dae::LivesLeftComponent::Update()
{
	if (!m_IsHit)
		return;

	m_InvincibleTime += Timer::GetInstance().GetDeltaTime();

	constexpr int maxInvincibleTime{ 2 };


	if (m_InvincibleTime < maxInvincibleTime)
		return;

	m_InvincibleTime = 0;
	m_IsHit = false;
}


void dae::LivesLeftComponent::SetupRequiredComponents()
{
	auto owner = GetOwner();
	if (!m_pTextComponent)
	{
		m_pTextComponent = owner->GetComponent<TextComponent>().get();

		if (!m_pTextComponent)
		{
			m_pTextComponent = owner->AddComponent<TextComponent>().get();
		}
	}
}

void dae::LivesLeftComponent::CheckForRequiredComponents()
{
	if (!m_pTextComponent)
	{
		throw std::invalid_argument("TextRendererComponent needs a TextComponent, currently nullptr");
	}
}

void dae::LivesLeftComponent::SetLivesLeftText()
{
	std::string text = std::to_string(m_LivesLeft);
	m_pTextComponent->SetText(text);

}

void dae::LivesLeftComponent::LoseLife(const Event* e)
{
	if (m_IsHit)
		return;

	if (!(strcmp(e->eventType, "PlayerDied") == 0))
		return;

	if (const PlayerEvent* event = dynamic_cast<const PlayerEvent*>(e))
	{
		if (event->playerIndex == m_PlayerIndex)
		{
			m_LivesLeft -= 1;

			SetLivesLeftText();

			std::unique_ptr<PlayerEvent> respawnEvent = std::make_unique<PlayerEvent>();
			respawnEvent->eventType = "PlayerRespawn";
			respawnEvent->playerIndex = m_PlayerIndex;

			EventManager::GetInstance().SendEventMessage(std::move(respawnEvent));

			m_IsHit = true;

		}
	}

	if (m_LivesLeft >= 0)
		return;

	std::unique_ptr<SceneEvent> event = std::make_unique<SceneEvent>();
	event->eventType = "OutOfLives";
	event->sceneName = SceneManager::GetInstance().GetActiveScene().GetName();

	EventManager::GetInstance().SendEventMessage(std::move(event));
}