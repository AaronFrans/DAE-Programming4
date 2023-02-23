#include "TransformComponent.h"


dae::TransformComponent::TransformComponent()
	: Component()
	, m_position{}
{
}

void dae::TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}
