#include <cassert>
#include "FpsCounterComponent.h"
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
	assert(m_Owner.lock()->GetComponent<TextComponent>()
		&& "FpsCounterComponent needs a text component");
}

void dae::FpsCounterComponent::Update()
{
	CheckForRequiredComponents();

	SetupRequiredComponents();

	m_Text.lock()->SetText(std::to_string(Timer::GetInstance().GetFPS()));
}

void dae::FpsCounterComponent::SetupRequiredComponents()
{
	if (m_Text.expired())
	{
		m_Text = m_Owner.lock()->GetComponent<TextComponent>();
	}
}


