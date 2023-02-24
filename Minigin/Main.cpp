#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "FpsCounterComponent.h"
#include "RenderComponent.h"
#include "TextRendererComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto textObj = std::make_shared<dae::GameObject>();
	auto textComp = textObj->AddComponent<dae::TextComponent>();
	auto transformComp = textObj->AddComponent<dae::TransformComponent>();
	textObj->AddComponent<dae::FpsCounterComponent>();
	textObj->AddComponent<dae::TextRendererComponent>();

	//test removing an UpdatingComponent
	//textObj->RemoveComponent<dae::FpsCounterComponent>();
    //textObj->AddComponent<dae::FpsCounterComponent>();
	//test removing a RenderingComponent
	//textObj->RemoveComponent<dae::FpsCounterComponent>();
    //textObj->AddComponent<dae::FpsCounterComponent>();


	textComp->SetFont(font);
	transformComp->SetPosition(0, 0, 0);

	//auto go = std::make_shared<dae::GameObject>();
	//go->SetTexture("background.tga");
	//scene.Add(go);
	//
	//go = std::make_shared<dae::GameObject>();
	//go->SetTexture("logo.tga");
	//go->SetPosition(216, 180);
	//scene.Add(go);
	//
	//to->SetPosition(80, 20);

	scene.Add(textObj);
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}