#include "CollisionComponent.h"
#include "Components/TransformComponent.h"

dae::CollisionComponent::CollisionComponent(std::weak_ptr<GameObject> owner)
	:Component(owner)
{
	m_pOwnerTransform = owner.lock()->GetTransform().get();

}

void dae::CollisionComponent::SetBounds(float width, float height)
{
	m_Width = width;
	m_Height = height;
}

