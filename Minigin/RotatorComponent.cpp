#include "RotatorComponent.h"
#include "TransformComponent.h"
#include "Timer.h"

dae::RotatorComponent::RotatorComponent(std::weak_ptr<GameObject> owner)
	: Component(owner)
{
	SetupRequiredComponents();
	CheckForRequiredComponents();

}

void dae::RotatorComponent::Update()
{
	CheckForRequiredComponents();

	

	auto curCircleProgress = m_LastCircleProgress + m_RotateSpeed * Timer::GetInstance().GetTimeStep();
	if (curCircleProgress != m_LastCircleProgress)
	{
		auto transformLocked = m_Transform.lock();
		auto parent = GetOwner().lock()->GetParent();

		if (!parent.expired())
		{
			glm::vec3 newPos = { cos(curCircleProgress) * m_CircleRadius ,sin(curCircleProgress) * m_CircleRadius, 0 };

			transformLocked->SetLocalPosition(newPos);
		}
		else
		{
			auto pivot = transformLocked->GetWorldPosition();
			glm::vec3 newPos = { pivot.x + cos(curCircleProgress) * m_CircleRadius , pivot.y + sin(curCircleProgress) * m_CircleRadius, 0 };

			transformLocked->SetLocalPosition(newPos);
		}

		m_LastCircleProgress = curCircleProgress;
		if (m_LastCircleProgress >= MAX_ROTATION)
		{
			m_LastCircleProgress -= MAX_ROTATION;
		}
	}
}

void dae::RotatorComponent::SetRotateSpeed(float speed)
{
	m_RotateSpeed = speed;
}

void dae::RotatorComponent::SetCircleRadius(float radius)
{
	m_CircleRadius = radius;
}


void dae::RotatorComponent::CheckForRequiredComponents() const
{
	if (m_Transform.expired())
	{
		throw std::invalid_argument("RotatorComponent needs a TransformComponent, currently expired");
	}
}

void dae::RotatorComponent::SetupRequiredComponents()
{
	if (m_Transform.expired())
	{
		auto lockedOwner = GetOwner().lock();
		m_Transform = lockedOwner->GetComponent<TransformComponent>();

		if (m_Transform.expired())
		{
			m_Transform = lockedOwner->AddComponent<TransformComponent>();
		}
	}
}
