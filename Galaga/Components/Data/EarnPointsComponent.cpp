#include "EarnPointsComponent.h"
#include "Events/EventManager.h"

dae::EarnPointsComponent::EarnPointsComponent(GameObject* owner)
	:Component(owner)
{
}

void dae::EarnPointsComponent::SetPlayerIndex(unsigned playerIndex)
{
	m_PlayerIndex = playerIndex;
}

void dae::EarnPointsComponent::EarnPoints()
{
	std::unique_ptr<PlayerEvent> event = std::make_unique<PlayerEvent>();
	event->eventType = "EnemyDied";
	event->playerIndex = m_PlayerIndex;

	EventManager::GetInstance().SendEventMessage<PlayerEvent>(std::move(event));
}
