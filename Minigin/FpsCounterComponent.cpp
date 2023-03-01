#include <stdexcept>
#include "FpsCounterComponent.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "Timer.h"

dae::FpsCounterComponent::FpsCounterComponent(std::weak_ptr<GameObject> owner)
	:UpdatingComponent(owner)
{
	SetupRequiredComponents();
	CheckForRequiredComponents();
}

void dae::FpsCounterComponent::CheckForRequiredComponents() const
{
	if (!m_Owner.lock()->GetComponent<TextComponent>())
	{
		throw std::invalid_argument("FpsCounter needs a Textcomponent");
	}
}

void dae::FpsCounterComponent::Update()
{
	CheckForRequiredComponents();

	int newFps = static_cast<int>(1.0f / Timer::GetInstance().GetTimeStep());

	if (newFps != m_LastFps)
	{
		m_LastFps = newFps;
		m_Text.lock()->SetText(std::to_string(newFps));
	}
}

void dae::FpsCounterComponent::SetupRequiredComponents()
{
	if (m_Text.expired())
	{
		auto lockedOwner = m_Owner.lock();
		m_Text = lockedOwner->GetComponent<TextComponent>();

		if (m_Text.expired())
		{
			m_Text = lockedOwner->AddComponent<TextComponent>();
		}
	}
}


