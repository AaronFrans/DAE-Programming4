#include "HighScoreComponent.h"

#include "FileReading/FileReader.h"
#include "Rendering/ResourceManager.h"

#include "Components/TextRendererComponent.h"
#include "Components/TextComponent.h"
#include "Components/TransformComponent.h"

#include "Scene/Scene.h"

#include "Events/GameEvents.h"
#include <Events/EventManager.h>

#include <algorithm>

dae::HighScoreComponent::HighScoreComponent(GameObject* owner)
	:Component(owner)
{
	m_Scores = FileReader::GetInstance().ReadHighscores("JsonFiles/HighScores.json");
	std::sort(m_Scores.begin(), m_Scores.end(), std::greater<int>());



	auto boundEnemySpawned = std::bind(&HighScoreComponent::FinalScoreSent, this, std::placeholders::_1);
	PointEvent spawnEvent;
	spawnEvent.eventType = "FinalScore";
	EventManager::GetInstance().AddObserver(spawnEvent, boundEnemySpawned);
}

dae::HighScoreComponent::~HighScoreComponent()
{
	FileReader::GetInstance().WriteHighscores("JsonFiles/HighScores.json", m_Scores);
}

void dae::HighScoreComponent::ReplaceChildren()
{
	auto pOwner = GetOwner();
	pOwner->ClearChildren();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Galaga.ttf", 15);
	auto pOwnerTransform = pOwner->GetTransform();

	constexpr int maxScoresCounter{ 5 };
	int scoreCounter{ 0 };

	for (auto& score : m_Scores)
	{
		if (scoreCounter == maxScoresCounter)
			break;

		auto scoreDisplay = std::make_shared<GameObject>();
		scoreDisplay->Init();

		auto pText = scoreDisplay->AddComponent<TextComponent>();
		pText->SetText("Score " + std::to_string(scoreCounter + 1) + ": " + std::to_string(score));
		pText->SetFont(font);
		scoreDisplay->SetParent(pOwner, false);

		scoreDisplay->GetTransform()->SetLocalPosition({ 0, 30 + 30 * scoreCounter, 0 });


		scoreDisplay->AddComponent<TextRendererComponent>();

		++scoreCounter;
	}
}

void dae::HighScoreComponent::FinalScoreSent(const Event* e)
{

	if (!(strcmp(e->eventType, "FinalScore") == 0))
		return;

	if (const PointEvent* event = dynamic_cast<const PointEvent*>(e))
	{
		int points = event->nrPoints;

		m_Scores.push_back(points);

		std::sort(m_Scores.begin(), m_Scores.end(), std::greater<int>());

		SceneManager::GetInstance().SetActiveScene("Highscores");

		ReplaceChildren();
	}


}

