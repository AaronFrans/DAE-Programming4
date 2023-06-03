#include <stdexcept>
#include "FpsCounterComponent.h"
#include "Components/TextComponent.h"
#include "Engine/GameObject.h"
#include "Engine/Timer.h"

dae::FpsCounterComponent::FpsCounterComponent(GameObject* owner)
	:Component(owner)
{
	SetupRequiredComponents();
	CheckForRequiredComponents();
}

void dae::FpsCounterComponent::CheckForRequiredComponents() const
{
	if (!m_Text)
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
		m_Text->SetText(std::to_string(newFps));
	}
}

void dae::FpsCounterComponent::SetupRequiredComponents()
{
	if (!m_Text)
	{
		auto owner = GetOwner();
		m_Text = owner->GetComponent<TextComponent>().get();

		if (!m_Text)
		{
			m_Text = owner->AddComponent<TextComponent>().get();
		}
	}
}


