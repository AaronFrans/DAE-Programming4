#include "HealthComponent.h"
#include "Events/EventManager.h"
#include <iostream>

dae::HealthComponent::HealthComponent(std::weak_ptr<GameObject> owner)
	:Component(owner)
{
}

void dae::HealthComponent::TakeDamage(float damage)
{
	m_Health -= damage;

	if (m_Health <= 0)
	{
		OutOfHealth();
	}
}

void dae::HealthComponent::SetPlayerIndex(unsigned playerIndex)
{
	m_PlayerIndex = playerIndex;
}

void dae::HealthComponent::OutOfHealth()
{
	std::unique_ptr<PlayerEvent> event = std::make_unique<PlayerEvent>();
	event->eventType = "PlayerDied";
	event->playerIndex = m_PlayerIndex;


	EventManager::GetInstance().SendEventMessage<PlayerEvent>(std::move(event));
}
