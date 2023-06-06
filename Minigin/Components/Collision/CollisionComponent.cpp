#include "CollisionComponent.h"
#include "Components/TransformComponent.h"
#include "Rendering/Renderer.h"

#include <iostream>
#include <cassert>

dae::CollisionComponent::CollisionComponent(GameObject* owner)
	:Component(owner)
{
	m_pOwnerTransform = owner->GetTransform().get();
}

dae::CollisionComponent::~CollisionComponent()
{
	assert(m_pScene && "m_pScene must be set in collision Component");
	m_pScene->RemoveCollision(this);

}

void dae::CollisionComponent::SetBounds(float width, float height)
{
	m_Width = width;
	m_Height = height;
}

dae::CollisionData dae::CollisionComponent::GetCollisionData() const
{
	return m_CollisionData;
}

void dae::CollisionComponent::SetCollisionData(dae::CollisionData data)
{
	m_CollisionData = data;
}

void dae::CollisionComponent::SetCallback(std::function<void(const CollisionData& owner, const CollisionData& hitObject)> callbackFunc)
{
	m_OnHitCallback = callbackFunc;
}

void dae::CollisionComponent::SetScene(Scene* scene)
{
	m_pScene = scene;
}

void dae::CollisionComponent::IsOverlappingOtherCollision(const std::vector<CollisionComponent*>& collisionsToCheck) const
{
	auto collisionOwner = GetOwner();
	for (const auto collisionToCheck : collisionsToCheck)
	{
		//will get rid of lock soon
		if (collisionOwner == collisionToCheck->GetOwner())
			continue;

		if (!CheckOverlapp(collisionToCheck))
			continue;

		//assert(m_OnHitCallback && "No Callback funtion set");

		if (!m_OnHitCallback)
			continue;

		m_OnHitCallback(m_CollisionData, collisionToCheck->m_CollisionData);
	}
}


bool dae::CollisionComponent::CheckOverlapp(CollisionComponent* pOther) const
{
	const auto worldPos = m_pOwnerTransform->GetWorldPosition();
	const auto otherWorldPos = pOther->m_pOwnerTransform->GetWorldPosition();


	const float left = worldPos.x;
	const float right = worldPos.x + m_Width;
	const float top = worldPos.y;
	const float bottom = worldPos.y + m_Height;

	const float otherLeft = otherWorldPos.x;
	const float otherRight = otherWorldPos.x + pOther->m_Width;
	const float otherTop = otherWorldPos.y;
	const float otherBottom = otherWorldPos.y + pOther->m_Height;


	bool overlapX = (left <= otherRight) && (right >= otherLeft);
	bool overlapY = (top <= otherBottom) && (bottom >= otherTop);

	return overlapX && overlapY;
}

#if _DEBUG
void dae::CollisionComponent::Render() const
{

	if (!m_DrawDebugLines)
		return;

	auto worldPos = m_pOwnerTransform->GetWorldPosition();

	Renderer::GetInstance().RenderRect(worldPos.x, worldPos.y,
		m_Width, m_Height,
		{ 150, 0, 2, 100 });
}
#endif

