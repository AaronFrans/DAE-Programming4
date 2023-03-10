#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Renderer.h"

#include "FpsCounterComponent.h"
#include "TextRendererComponent.h"
#include "ImageRenderComponent.h"
#include "TransformComponent.h"
#include "TextComponent.h"
#include "ImageComponent.h"
#include "RotatorComponent.h"

void load()
{
	//TODO: test remove transform
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto bgImage = std::make_shared<dae::GameObject>();
	//call init to cache transform
	bgImage->Init();
	bgImage->AddComponent<dae::ImageRenderComponent>();
	bgImage->GetComponent<dae::ImageComponent>()->SetTexture("background.tga");
	scene.Add(bgImage);


	auto logoImage = std::make_shared<dae::GameObject>();
	logoImage->Init();
	logoImage->AddComponent<dae::ImageRenderComponent>();
	logoImage->GetComponent<dae::ImageComponent>()->SetTexture("logo.tga");
	logoImage->GetTransform()->SetLocalPosition({216, 180, 0});

	//Test adding and removing child
	logoImage->SetParent(bgImage, true);
	logoImage->SetParent(nullptr, false);
	scene.Add(logoImage);


	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto textObj = std::make_shared<dae::GameObject>();
	textObj->Init();
	textObj->AddComponent<dae::FpsCounterComponent>();
	textObj->AddComponent<dae::TextRendererComponent>();

	//test removing an UpdatingComponent
	//textObj->RemoveComponent<dae::FpsCounterComponent>();
	//textObj->AddComponent<dae::FpsCounterComponent>();
	//test removing a RenderingComponent
	//textObj->RemoveComponent<dae::FpsCounterComponent>();
	//textObj->AddComponent<dae::FpsCounterComponent>();

	textObj->GetComponent<dae::TextComponent>()->SetFont(font);
	textObj->GetTransform()->SetLocalPosition({0, 0, 0});

	scene.Add(textObj);

	auto butterfly = std::make_shared<dae::GameObject>();
	butterfly->Init();
	butterfly->AddComponent<dae::RotatorComponent>();
	butterfly->AddComponent<dae::ImageRenderComponent>();

	const glm::vec3 butterflyPivot = { 320, 350, 0 };
	auto butterflyRotator = butterfly->GetComponent<dae::RotatorComponent>();
	butterflyRotator->SetRotateSpeed(3);
	butterflyRotator->SetPivot({ 320, 325, 0 });
	butterfly->GetTransform()->SetLocalPosition({ 320, 325, 0 });
	butterfly->GetComponent<dae::ImageComponent>()->SetTexture("Galaga_Butterfly.png");

	scene.Add(butterfly);


	auto bee = std::make_shared<dae::GameObject>();
	bee->Init();
	bee->AddComponent<dae::RotatorComponent>()->SetRotateSpeed(-6);
	bee->AddComponent<dae::ImageRenderComponent>();

	bee->GetComponent<dae::ImageComponent>()->SetTexture("Galaga_Bee.png");

	bee->SetParent(butterfly, false);



	scene.Add(bee);


}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}