#include "RestrictedMovementCommand.h"
#include "Engine/GameObject.h"
#include "Engine/Timer.h"

#include "Components/TransformComponent.h"

#include "Events/EventManager.h"

#include "Scene/SceneManager.h"

dae::RestrictedMovementCommand::RestrictedMovementCommand(GameObject* actor)
{
	m_pGameObjectTransform = actor->GetTransform().get();


	auto boundPlayerGrabbed = std::bind(&RestrictedMovementCommand::PlayerGrabbed, this, std::placeholders::_1);
	PlayerEvent grabbedEvent;
	grabbedEvent.eventType = "PlayerGrabbed";
	EventManager::GetInstance().AddObserver(grabbedEvent, boundPlayerGrabbed);

	auto boundPlayerRespawn = std::bind(&RestrictedMovementCommand::PlayerRespawn, this, std::placeholders::_1);
	PlayerEvent respawnEvent;
	respawnEvent.eventType = "PlayerRespawn";
	EventManager::GetInstance().AddObserver(respawnEvent, boundPlayerRespawn);
}

dae::RestrictedMovementCommand::~RestrictedMovementCommand()
{
	auto boundPlayerGrabbed = std::bind(&RestrictedMovementCommand::PlayerGrabbed, this, std::placeholders::_1);
	PlayerEvent grabbedEvent;
	grabbedEvent.eventType = "PlayerGrabbed";
	EventManager::GetInstance().RemoveObserver(grabbedEvent, boundPlayerGrabbed);

	auto boundPlayerRespawn = std::bind(&RestrictedMovementCommand::PlayerRespawn, this, std::placeholders::_1);
	PlayerEvent respawnEvent;
	respawnEvent.eventType = "PlayerRespawn";
	EventManager::GetInstance().RemoveObserver(respawnEvent, boundPlayerRespawn);

}

void dae::RestrictedMovementCommand::Execute()
{

	if (!m_CanPlayerMove)
		return;

	auto newPos = m_pGameObjectTransform->GetLocalPosition() + m_Direction * m_MoveSpeed * Timer::GetInstance().GetDeltaTime();

	newPos = glm::clamp(newPos, m_MinPoint, m_MaxPoint);

	m_pGameObjectTransform->SetLocalPosition(newPos);
}

void dae::RestrictedMovementCommand::PlayerGrabbed(const Event* e)
{
	if (strcmp(e->eventType, "PlayerGrabbed") != 0)
		return;

	if (const PlayerEvent* event = dynamic_cast<const PlayerEvent*>(e))
	{
		if (event->playerIndex != m_PlayerIndex)
			return;

		m_CanPlayerMove = false;
	}

}

void dae::RestrictedMovementCommand::PlayerRespawn(const Event* e)
{
	if (strcmp(e->eventType, "PlayerRespawn") != 0)
		return;

	if (const PlayerEvent* event = dynamic_cast<const PlayerEvent*>(e))
	{
		if (event->playerIndex != m_PlayerIndex)
			return;

		m_CanPlayerMove = true;
	}

}