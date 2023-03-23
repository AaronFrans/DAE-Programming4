#include <stdexcept>
#include "FpsCounterComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Timer.h"

dae::FpsCounterComponent::FpsCounterComponent(std::weak_ptr<GameObject> owner)
	:Component(owner)
{
	SetupRequiredComponents();
	CheckForRequiredComponents();
}

void dae::FpsCounterComponent::CheckForRequiredComponents() const
{
	if (m_Text.expired())
	{
		throw std::invalid_argument("FpsCounter needs a Textcomponent, currently expired");
	}
}

void dae::FpsCounterComponent::Update()
{

	int newFps = static_cast<int>(1.0f / Timer::GetInstance().GetDeltaTime());

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
		auto lockedOwner = GetOwner().lock();
		m_Text = lockedOwner->GetComponent<TextComponent>();

		if (m_Text.expired())
		{
			m_Text = lockedOwner->AddComponent<TextComponent>();
		}
	}
}


