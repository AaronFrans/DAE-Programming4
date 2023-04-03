#include "PlayerScoreComponent.h"
#include "TextComponent.h"
#include "EventManager.h"
#include <memory>

dae::PlayerScoreComponent::PlayerScoreComponent(std::weak_ptr<GameObject> owner)
	:Component(owner)
{
	SetupRequiredComponents();
	CheckForRequiredComponents();

	SetPointsText();

	auto boundLoseLife = std::bind(&PlayerScoreComponent::EarnPoints, this, std::placeholders::_1);
	PlayerEvent event;
	event.eventType = "EnemyDied";
	EventManager::GetInstance().AddObserver(event, boundLoseLife);
}

void dae::PlayerScoreComponent::SetPlayerIndex(unsigned playerIndex)
{
	m_PlayerIndex = playerIndex;
}

void dae::PlayerScoreComponent::SetupRequiredComponents()
{
	auto lockedOwner = GetOwner().lock();
	if (!m_pTextComponent)
	{
		m_pTextComponent = lockedOwner->GetComponent<TextComponent>().get();

		if (!m_pTextComponent)
		{
			m_pTextComponent = lockedOwner->AddComponent<TextComponent>().get();
		}
	}
}

void dae::PlayerScoreComponent::CheckForRequiredComponents()
{
	if (!m_pTextComponent)
	{
		throw std::invalid_argument("TextRendererComponent needs a TextComponent, currently expired");
	}
}

void dae::PlayerScoreComponent::SetPointsText()
{
	std::string text = "Score: " + std::to_string(m_PointsEarned);
	m_pTextComponent->SetText(text);
}

void dae::PlayerScoreComponent::EarnPoints(const Event* e)
{
	if (strcmp(e->eventType, "EnemyDied") == 0)
	{
		if (const PlayerEvent* event = dynamic_cast<const PlayerEvent*>(e))
		{
			if (event->playerIndex == m_PlayerIndex)
			{
				m_PointsEarned += POINT_FOR_ENEMY;

				SetPointsText();
			}
		}
	}
}
