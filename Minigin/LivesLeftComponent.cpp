#include "LivesLeftComponent.h"
#include "EventManager.h"
#include "TextComponent.h"
#include <memory>

dae::LivesLeftComponent::LivesLeftComponent(std::weak_ptr<GameObject> owner)
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

void dae::LivesLeftComponent::SetPlayerIndex(unsigned playerIndex)
{
	m_PlayerIndex = playerIndex;
}


void dae::LivesLeftComponent::SetupRequiredComponents()
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

void dae::LivesLeftComponent::CheckForRequiredComponents()
{
	if (!m_pTextComponent)
	{
		throw std::invalid_argument("TextRendererComponent needs a TextComponent, currently expired");
	}
}

void dae::LivesLeftComponent::SetLivesLeftText()
{
	std::string text = "Lives Left: " + std::to_string(m_LivesLeft);
	m_pTextComponent->SetText(text);
}

void dae::LivesLeftComponent::LoseLife(const Event* e)
{
	if (strcmp(e->eventType, "PlayerDied") == 0)
	{
		if (const PlayerEvent* event = dynamic_cast<const PlayerEvent*>(e))
		{
			if (event->playerIndex == m_PlayerIndex)
			{
				m_LivesLeft -= 1;

				SetLivesLeftText();
			}
		}
	}
}