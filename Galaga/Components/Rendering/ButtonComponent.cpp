#include "ButtonComponent.h"
#include "Components/TextRendererComponent.h"
#include "Components/TextComponent.h"
#include "Rendering/ResourceManager.h"

dae::ButtonComponent::ButtonComponent(GameObject* owner)
	:Component(owner)
{

}

void dae::ButtonComponent::SetActive(bool isActive)
{
	m_ActiveRender->SetActive(isActive);
	m_InactiveRender->SetActive(!isActive);
}

void dae::ButtonComponent::DoClick()
{
	m_OnClick();
}

void dae::ButtonComponent::SetButtonText(const std::string& text)
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Galaga.ttf", 15);
	auto pOwner = GetOwner();


	auto activeChild = std::make_shared<GameObject>();
	activeChild->Init();
	auto pActiveText = activeChild->AddComponent<TextComponent>();
	pActiveText->SetColor({ 255, 255, 30 });
	pActiveText->SetFont(font);
	pActiveText->SetText(text);
	m_ActiveRender = activeChild->AddComponent<TextRendererComponent>().get();
	activeChild->SetParent(pOwner, false);




	auto inactiveChild = std::make_shared<GameObject>();
	inactiveChild->Init();
	auto pInactiveText = inactiveChild->AddComponent<TextComponent>();
	pInactiveText->SetColor({ 255, 255, 255 });
	pInactiveText->SetFont(font);
	pInactiveText->SetText(text);
	m_InactiveRender = inactiveChild->AddComponent<TextRendererComponent>().get();

	inactiveChild->SetParent(pOwner, false);

}

void dae::ButtonComponent::SetOnClick(std::function<void()> onClick)
{
	m_OnClick = onClick;
}
