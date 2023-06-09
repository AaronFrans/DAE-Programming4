#include "GameOverseerComponent.h"
#include <functional>

#include "Events/GameEvents.h"
#include <Events/EventManager.h>
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
}

void dae::GameOverseerComponent::EnemyDead(const Event* e)
{

	std::cout << "Enemy Dead";
	if (!(strcmp(e->eventType, "EnemyDied") == 0))
		return;

	--m_NrEnemies;

	if (m_NrEnemies != 0)
		return;

	std::unique_ptr<Event> event = std::make_unique<Event>();
	event->eventType = "AllEnemiesDead";
	EventManager::GetInstance().SendEventMessage(std::move(event));
}

void dae::GameOverseerComponent::EnemySpawned(const Event* e)
{
	
	std::cout << "Enemy Spawned";
	if (!(strcmp(e->eventType, "EnemySpawned") == 0))
		return;

	++m_NrEnemies;
}


