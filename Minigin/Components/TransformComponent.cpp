#include "TransformComponent.h"



dae::TransformComponent::TransformComponent(GameObject* owner)
	:Component(owner)
{
}

const glm::vec3& dae::TransformComponent::GetWorldPosition()
{
	if (m_IsDirty)
		UpdateWorldPosition();
	return m_WorldPostition;
}

const glm::vec3& dae::TransformComponent::GetLocalPosition() const
{
	return m_LocalPostition;
}

void dae::TransformComponent::UpdateWorldPosition()
{
	const auto Owner = GetOwner();
	if (Owner->GetParent())
		m_WorldPostition = m_LocalPostition;
	else
	{
		const auto parentTransform = Owner->GetParent()->GetTransform();
		m_WorldPostition = parentTransform.get()->GetWorldPosition() + m_LocalPostition;
	}
	m_IsDirty = false;
}

void dae::TransformComponent::AddToLocalPosition(const glm::vec3& v)
{

	m_LocalPostition += v;
	SetPositionDirty();

}

void dae::TransformComponent::SetLocalPosition(const glm::vec3& newPos)
{
	m_LocalPostition = newPos;
	SetPositionDirty();

}

void dae::TransformComponent::SetPositionDirty()
{
	m_IsDirty = true;

	//make owner raw pointer? or keep locked?
	for (auto& child : GetOwner()->GetChildren())
	{
		child->GetTransform()->SetPositionDirty();
	}
}
