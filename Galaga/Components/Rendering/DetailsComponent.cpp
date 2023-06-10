#include "DetailsComponent.h"
#include "Components/TextRendererComponent.h"
#include "Components/TextComponent.h"
#include "Components/TransformComponent.h"
#include "Rendering/ResourceManager.h"
#include "Scene/SceneManager.h"
#include <Events/EventManager.h>
#include <Events/GameEvents.h>
#include <iostream>


dae::DetailsComponent::DetailsComponent(GameObject* owner)
	:Component(owner)
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Galaga.ttf", 15);
	auto pOwner = GetOwner();

	constexpr int textHeightOffset{ 30 };

	auto shotsGO = std::make_shared<GameObject>();
	shotsGO->Init();

	m_ShotsText = shotsGO->AddComponent<TextComponent>().get();
	m_ShotsText->SetColor({ 255, 255, 30 });
	m_ShotsText->SetFont(font);
	m_ShotsText->SetText("Nr of Shots: ");

	m_ShotsRender = shotsGO->AddComponent<TextRendererComponent>().get();
	shotsGO->SetParent(pOwner, false);


	auto hitsGO = std::make_shared<GameObject>();
	hitsGO->Init();

	m_HitText = hitsGO->AddComponent<TextComponent>().get();
	m_HitText->SetColor({ 255, 255, 255 });
	m_HitText->SetFont(font);
	m_HitText->SetText("Nr of Hits: ");

	m_HitRender = hitsGO->AddComponent<TextRendererComponent>().get();
	hitsGO->SetParent(pOwner, false);
	hitsGO->GetTransform()->SetLocalPosition({ 0,textHeightOffset * 1,0 });




	auto percentageGO = std::make_shared<GameObject>();
	percentageGO->Init();

	m_PercentText = percentageGO->AddComponent<TextComponent>().get();
	m_PercentText->SetColor({ 255, 255, 30 });
	m_PercentText->SetFont(font);
	m_PercentText->SetText("Percentage of hits: %");

	m_PercentRender = percentageGO->AddComponent<TextRendererComponent>().get();
	percentageGO->SetParent(pOwner, false);
	percentageGO->GetTransform()->SetLocalPosition({ 0,textHeightOffset * 2,0 });
}

void dae::DetailsComponent::SetActive(bool isActive)
{
	m_ShotsRender->SetActive(isActive);
	m_HitRender->SetActive(isActive);
	m_PercentRender->SetActive(isActive);
}

void dae::DetailsComponent::GoToNextScene()
{

	
	if (m_NextSceneName == "Highscores")
	{
		SceneManager::GetInstance().SetActiveScene(m_NextSceneName);
		return;
	}

	SetActive(false);
	std::unique_ptr<SceneEvent> nextLevelEvent = std::make_unique<SceneEvent>();
	nextLevelEvent->sceneName = m_NextSceneName;
	nextLevelEvent->eventType = "NextLevel";
	EventManager::GetInstance().SendEventMessage(std::move(nextLevelEvent));

}

void dae::DetailsComponent::SetDetailsText(const int nrShots, const int nrHits)
{
	m_ShotsText->SetText("Nr of Shots: " + std::to_string(nrShots));

	m_HitText->SetText("Nr of Hits: " + std::to_string(nrHits));

	const int percentage = static_cast<int>(static_cast<double>(nrHits) / nrShots * 100.0);
	m_PercentText->SetText("Percentage of hits: %" + std::to_string(percentage));
}
