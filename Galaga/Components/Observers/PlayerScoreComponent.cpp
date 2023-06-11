#include "PlayerScoreComponent.h"
#include "Components/Component.h"
#include "Events/EventManager.h"
#include "Events/GameEvents.h"
#include "Components/TextComponent.h"
#include "Scene/SceneManager.h"
#include <memory>
#include <iostream>

dae::PlayerScoreComponent::PlayerScoreComponent(GameObject* owner)
	:Component(owner)
{
	SetupRequiredComponents();
	CheckForRequiredComponents();

	SetPointsText();

	auto boundLoseLife = std::bind(&PlayerScoreComponent::EarnPoints, this, std::placeholders::_1);
	PointEvent event;
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

	auto boundNextScene = std::bind(&PlayerScoreComponent::NextScene, this, std::placeholders::_1);
	Event nextLevelEvent;
	nextLevelEvent.eventType = "NextLevel";
	EventManager::GetInstance().AddObserver(nextLevelEvent, boundNextScene);

	auto boundNewScene = std::bind(&PlayerScoreComponent::NextSceneLoaded, this, std::placeholders::_1);
	Event newSceneEvent;
	newSceneEvent.eventType = "NextSceneScore";
	EventManager::GetInstance().AddObserver(newSceneEvent, boundNewScene);
}

dae::PlayerScoreComponent::~PlayerScoreComponent()
{
	auto boundLoseLife = std::bind(&PlayerScoreComponent::EarnPoints, this, std::placeholders::_1);
	PointEvent event;
	event.eventType = "EnemyDied";
	EventManager::GetInstance().RemoveObserver(event, boundLoseLife);

	auto boundOutOfLives = std::bind(&PlayerScoreComponent::FinalScore, this, std::placeholders::_1);
	Event outOfLivesEvent;
	outOfLivesEvent.eventType = "OutOfLives";
	EventManager::GetInstance().RemoveObserver(outOfLivesEvent, boundOutOfLives);


	auto boundEnemiesDead = std::bind(&PlayerScoreComponent::FinalScore, this, std::placeholders::_1);
	Event enemiesDeadEvent;
	enemiesDeadEvent.eventType = "AllEnemiesDead";
	EventManager::GetInstance().RemoveObserver(enemiesDeadEvent, boundEnemiesDead);

	auto boundNextScene = std::bind(&PlayerScoreComponent::NextScene, this, std::placeholders::_1);
	Event nextLevelEvent;
	nextLevelEvent.eventType = "NextLevel";
	EventManager::GetInstance().RemoveObserver(nextLevelEvent, boundNextScene);

	auto boundNewScene = std::bind(&PlayerScoreComponent::NextSceneLoaded, this, std::placeholders::_1);
	Event newSceneEvent;
	newSceneEvent.eventType = "NextSceneScore";
	EventManager::GetInstance().RemoveObserver(newSceneEvent, boundNewScene);
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

	if (const SceneEvent* event = dynamic_cast<const SceneEvent*>(e))
	{
		if (m_SceneName != event->sceneName)
			return;

		m_PointsEarned += event->nrPoints;

		SetPointsText();
	}
}

void dae::PlayerScoreComponent::FinalScore(const Event* e)
{
	if (!(strcmp(e->eventType, "OutOfLives") == 0) && !(strcmp(e->eventType, "AllEnemiesDead") == 0))
		return;

	if (const SceneEvent* scene = dynamic_cast<const SceneEvent*>(e))
	{
		if (scene->sceneName != m_SceneName)
			return;

		std::unique_ptr<PointEvent> event = std::make_unique<PointEvent>();
		event->eventType = "FinalScore";
		event->nrPoints = m_PointsEarned;
		EventManager::GetInstance().SendEventMessage(std::move(event));
	}
}



void dae::PlayerScoreComponent::NextScene(const Event* e)
{
	if (strcmp(e->eventType, "NextLevel") != 0)
		return;


	if (const SceneEvent* scene = dynamic_cast<const SceneEvent*>(e))
	{
		if (m_NextSceneName != scene->sceneName)
			return;

		std::unique_ptr<SceneEvent> event = std::make_unique<SceneEvent>();
		event->eventType = "NextSceneScore";
		event->nrPoints = m_PointsEarned;
		event->sceneName = m_NextSceneName;
		EventManager::GetInstance().SendEventMessage(std::move(event));
	}

}

void dae::PlayerScoreComponent::NextSceneLoaded(const Event* e)
{
	if (strcmp(e->eventType, "NextSceneScore") != 0)
		return;


	if (const SceneEvent* event = dynamic_cast<const SceneEvent*>(e))
	{
		if (m_SceneName != event->sceneName)
			return;

		SceneManager::GetInstance().SetActiveScene(m_SceneName);
		m_PointsEarned = event->nrPoints;

		SetPointsText();
	}
}
