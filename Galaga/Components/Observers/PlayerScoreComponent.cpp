#include "PlayerScoreComponent.h"
#include "Components/Component.h"
#include "Events/EventManager.h"
#include "Events/GameEvents.h"
#include "Components/TextComponent.h"
#include <memory>
#include <iostream>

dae::PlayerScoreComponent::PlayerScoreComponent(GameObject* owner)
	:Component(owner)
{
	SetupRequiredComponents();
	CheckForRequiredComponents();

	SetPointsText();

	auto boundLoseLife = std::bind(&PlayerScoreComponent::EarnPoints, this, std::placeholders::_1);
	PlayerEvent event;
	event.eventType = "EnemyDied";
	EventManager::GetInstance().AddObserver(event, boundLoseLife);

	auto boundOutOfLives = std::bind(&PlayerScoreComponent::FinalScore, this, std::placeholders::_1);
	Event outOfLivesEvent;
	outOfLivesEvent.eventType = "OutOfLives";
	EventManager::GetInstance().AddObserver(outOfLivesEvent, boundOutOfLives);


	auto boundEnemiesDead = std::bind(&PlayerScoreComponent::FinalScore, this, std::placeholders::_1);
	Event enemiesDeadEvent;
	enemiesDeadEvent.eventType = "AllEnemiesDead";
	EventManager::GetInstance().AddObserver(enemiesDeadEvent, boundEnemiesDead);
}

void dae::PlayerScoreComponent::SetPlayerIndex(unsigned playerIndex)
{
	m_PlayerIndex = playerIndex;
}


void dae::PlayerScoreComponent::SetupRequiredComponents()
{
	auto Owner = GetOwner();
	if (!m_pTextComponent)
	{
		m_pTextComponent = Owner->GetComponent<TextComponent>().get();

		if (!m_pTextComponent)
		{
			m_pTextComponent = Owner->AddComponent<TextComponent>().get();
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
	if (strcmp(e->eventType, "EnemyDied") != 0)
		return;

	if (const PointEvent* event = dynamic_cast<const PointEvent*>(e))
	{
		m_PointsEarned += event->nrPoints;

		SetPointsText();
	}
}

void dae::PlayerScoreComponent::FinalScore(const Event* e)
{
	std::cout << "Out of Lives || AllEnemiesDead";
	if (!(strcmp(e->eventType, "OutOfLives") == 0) && !(strcmp(e->eventType, "AllEnemiesDead") == 0))
		return;

	std::unique_ptr<PointEvent> event = std::make_unique<PointEvent>();
	event->eventType = "FinalScore";
	event->nrPoints = m_PointsEarned;
	EventManager::GetInstance().SendEventMessage(std::move(event));
}
