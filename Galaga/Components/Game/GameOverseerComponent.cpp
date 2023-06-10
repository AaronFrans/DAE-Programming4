#include "GameOverseerComponent.h"
#include <functional>

#include "Components/Rendering/DetailsComponent.h"

#include "Events/GameEvents.h"
#include <Events/EventManager.h>
#include <Scene/SceneManager.h>
#include <iostream>

dae::GameOverseerComponent::GameOverseerComponent(GameObject* owner)
	:Component(owner)
{
	auto boundEnemyDead = std::bind(&GameOverseerComponent::EnemyDead, this, std::placeholders::_1);
	PointEvent enemyDeadEvent;
	enemyDeadEvent.eventType = "EnemyDied";
	EventManager::GetInstance().AddObserver(enemyDeadEvent, boundEnemyDead);

	auto boundEnemySpawned = std::bind(&GameOverseerComponent::EnemySpawned, this, std::placeholders::_1);
	Event spawnEvent;
	spawnEvent.eventType = "EnemySpawned";
	EventManager::GetInstance().AddObserver(spawnEvent, boundEnemySpawned);

	auto boundEnemyHit = std::bind(&GameOverseerComponent::EnemyHit, this, std::placeholders::_1);
	Event hitEvent;
	hitEvent.eventType = "EnemyHit";
	EventManager::GetInstance().AddObserver(hitEvent, boundEnemyHit);

	auto boundBulletShot = std::bind(&GameOverseerComponent::BulletShot, this, std::placeholders::_1);
	Event shotEvent;
	shotEvent.eventType = "BulletFired";
	EventManager::GetInstance().AddObserver(shotEvent, boundBulletShot);
}

dae::GameOverseerComponent::~GameOverseerComponent()
{
	auto boundEnemyDead = std::bind(&GameOverseerComponent::EnemyDead, this, std::placeholders::_1);
	PointEvent enemyDeadEvent;
	enemyDeadEvent.eventType = "EnemyDied";
	EventManager::GetInstance().RemoveObserver(enemyDeadEvent, boundEnemyDead);

	auto boundEnemySpawned = std::bind(&GameOverseerComponent::EnemySpawned, this, std::placeholders::_1);
	Event spawnEvent;
	spawnEvent.eventType = "EnemySpawned";
	EventManager::GetInstance().RemoveObserver(spawnEvent, boundEnemySpawned);

	auto boundEnemyHit = std::bind(&GameOverseerComponent::EnemyHit, this, std::placeholders::_1);
	Event hitEvent;
	hitEvent.eventType = "EnemyHit";
	EventManager::GetInstance().RemoveObserver(hitEvent, boundEnemyHit);

	auto boundBulletShot = std::bind(&GameOverseerComponent::BulletShot, this, std::placeholders::_1);
	Event shotEvent;
	shotEvent.eventType = "BulletFired";
	EventManager::GetInstance().RemoveObserver(shotEvent, boundBulletShot);
}

void dae::GameOverseerComponent::SetDetails(DetailsComponent* details)
{
	m_pDetails = details;
}

void dae::GameOverseerComponent::SkipLevel()
{
	SceneManager::GetInstance().SetActiveScene(m_NextSceneName);
}

void dae::GameOverseerComponent::EnemyDead(const Event* e)
{


	if (!(strcmp(e->eventType, "EnemyDied") == 0))
		return;

	if (const SceneEvent* scene = dynamic_cast<const SceneEvent*>(e))
	{
		if (m_SceneName != scene->sceneName)
			return;

		--m_NrEnemies;

		if (m_NrEnemies != 0)
			return;
		std::cout << "All enemies dead\n";

		m_pDetails->SetActive(true);
		m_pDetails->SetDetailsText(m_NrShots, m_NrHits);
		m_pDetails->SetNextSceneName(m_NextSceneName);

		if (m_NextSceneName == "Highscores")
		{
			std::unique_ptr<SceneEvent> event = std::make_unique<SceneEvent>();
			event->eventType = "AllEnemiesDead";
			event->sceneName = m_SceneName;
			EventManager::GetInstance().SendEventMessage(std::move(event));
		}

		std::cout << "going to details\n";

		SceneManager::GetInstance().SetActiveScene("Details");

	}

}

void dae::GameOverseerComponent::EnemyHit(const Event* e)
{
	if (!(strcmp(e->eventType, "EnemyHit") == 0))
		return;

	if (const SceneEvent* scene = dynamic_cast<const SceneEvent*>(e))
	{
		if (m_SceneName != scene->sceneName)
			return;

		++m_NrHits;
	}
}

void dae::GameOverseerComponent::BulletShot(const Event* e)
{
	if (!(strcmp(e->eventType, "BulletFired") == 0))
		return;

	if (const SceneEvent* scene = dynamic_cast<const SceneEvent*>(e))
	{
		if (m_SceneName != scene->sceneName)
			return;

		++m_NrShots;

	}
}

void dae::GameOverseerComponent::EnemySpawned(const Event* e)
{
	if (!(strcmp(e->eventType, "EnemySpawned") == 0))
		return;

	if (const SceneEvent* scene = dynamic_cast<const SceneEvent*>(e))
	{
		if (m_SceneName != scene->sceneName)
			return;

		++m_NrEnemies;
	}
}


