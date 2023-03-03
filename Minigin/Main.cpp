#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "FpsCounterComponent.h"
#include "TextRendererComponent.h"
#include "ImageRenderComponent.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "ImageComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");


	auto bgImage = std::make_shared<dae::GameObject>();
	bgImage->AddComponent<dae::ImageRenderComponent>();
	bgImage->GetComponent<dae::ImageComponent>()->SetTexture("background.tga");
	scene.Add(bgImage);


	auto logoImage = std::make_shared<dae::GameObject>();
	logoImage->AddComponent<dae::ImageRenderComponent>();
	logoImage->GetComponent<dae::ImageComponent>()->SetTexture("logo.tga");
	logoImage->GetComponent<dae::TransformComponent>()->SetPosition(216, 180, 0);

	scene.Add(logoImage);
	

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto textObj = std::make_shared<dae::GameObject>();
	textObj->AddComponent<dae::FpsCounterComponent>();
	textObj->AddComponent<dae::TextRendererComponent>();

	//test removing an UpdatingComponent
	//textObj->RemoveComponent<dae::FpsCounterComponent>();
	//textObj->AddComponent<dae::FpsCounterComponent>();
	//test removing a RenderingComponent
	//textObj->RemoveComponent<dae::FpsCounterComponent>();
	//textObj->AddComponent<dae::FpsCounterComponent>();

	textObj->GetComponent<dae::TextComponent>()->SetFont(font);
	textObj->GetComponent<dae::TransformComponent>()->SetPosition(0, 0, 0);

	scene.Add(textObj);
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}