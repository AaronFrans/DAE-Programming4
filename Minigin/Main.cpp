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
#include "InputManager.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include "GameObject.h"

#include "FpsCounterComponent.h"
#include "TextRendererComponent.h"
#include "ImageRenderComponent.h"
#include "TransformComponent.h"
#include "TextComponent.h"
#include "ImageComponent.h"
#include "RotatorComponent.h"
#include "TrashTheCacheComponent.h"

#include "XboxController.h"
#include "MoveCommand.h"


void Rotators()
{
	//TODO: test remove transform
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Rotators");

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
	logoImage->GetTransform()->SetLocalPosition({ 216, 180, 0 });

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
	textObj->GetTransform()->SetLocalPosition({ 0, 0, 0 });

	scene.Add(textObj);


	auto pivot = std::make_shared<dae::GameObject>();
	pivot->Init();
	pivot->GetTransform()->SetLocalPosition({ 320, 350, 0 });

	scene.Add(pivot);


	auto butterfly = std::make_shared<dae::GameObject>();
	butterfly->Init();
	butterfly->AddComponent<dae::RotatorComponent>();
	butterfly->AddComponent<dae::ImageRenderComponent>();

	auto butterflyRotator = butterfly->GetComponent<dae::RotatorComponent>();
	butterflyRotator->SetRotateSpeed(3);
	butterfly->GetTransform()->SetLocalPosition({ 320, 325, 0 });
	butterfly->GetComponent<dae::ImageComponent>()->SetTexture("Galaga_Butterfly.png");

	butterfly->SetParent(pivot, false);


	auto bee = std::make_shared<dae::GameObject>();
	bee->Init();
	bee->AddComponent<dae::RotatorComponent>()->SetRotateSpeed(-6);
	bee->AddComponent<dae::ImageRenderComponent>();

	bee->GetComponent<dae::ImageComponent>()->SetTexture("Galaga_Bee.png");

	bee->SetParent(butterfly, false);

	auto ttc = std::make_shared<dae::GameObject>();
	ttc->Init();

	ttc->AddComponent<dae::TrashTheCacheComponent>();

	scene.Add(ttc);


}


void Input()
{
	//TODO: test remove transform
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Input");
	auto& input = dae::InputManager::GetInstance();

	unsigned controller = input.AddController();

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
	logoImage->GetTransform()->SetLocalPosition({ 216, 180, 0 });


	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto textObj = std::make_shared<dae::GameObject>();
	textObj->Init();
	textObj->AddComponent<dae::FpsCounterComponent>();
	textObj->AddComponent<dae::TextRendererComponent>();


	textObj->GetComponent<dae::TextComponent>()->SetFont(font);
	textObj->GetTransform()->SetLocalPosition({ 0, 0, 0 });

	scene.Add(textObj);

	auto butterfly = std::make_shared<dae::GameObject>();
	butterfly->Init();
	butterfly->AddComponent<dae::ImageRenderComponent>();

	butterfly->GetTransform()->SetLocalPosition({ 320, 325, 0 });
	butterfly->GetComponent<dae::ImageComponent>()->SetTexture("Galaga_Butterfly.png");

	constexpr float butterflyMoveSpeed{ 60 };

	auto pCommand = input.AddXboxCommand<dae::MoveCommand>(std::make_unique<dae::MoveCommand>(butterfly.get()),
		dae::XboxControllerInput{controller, dae::XboxController::ControllerButton::DPadUp, dae::ButtonState::Pressed});

	pCommand->SetDirection({ 0,1,0 });
	pCommand->SetMoveSpeed(-butterflyMoveSpeed);

	pCommand = input.AddXboxCommand<dae::MoveCommand>(std::make_unique<dae::MoveCommand>(butterfly.get()),
		dae::XboxControllerInput{ controller, dae::XboxController::ControllerButton::DPadDown,dae::ButtonState::Pressed });
	pCommand->SetDirection({ 0,1,0 });
	pCommand->SetMoveSpeed(butterflyMoveSpeed);

	pCommand = input.AddXboxCommand<dae::MoveCommand>(std::make_unique<dae::MoveCommand>(butterfly.get()),
		dae::XboxControllerInput{ controller, dae::XboxController::ControllerButton::DPadRight, dae::ButtonState::Pressed });
	pCommand->SetDirection({ 1,0,0 });
	pCommand->SetMoveSpeed(butterflyMoveSpeed);

	pCommand = input.AddXboxCommand<dae::MoveCommand>(std::make_unique<dae::MoveCommand>(butterfly.get()),
		dae::XboxControllerInput{ controller, dae::XboxController::ControllerButton::DPadLeft, dae::ButtonState::Pressed });
	pCommand->SetDirection({ 1,0,0 });
	pCommand->SetMoveSpeed(-butterflyMoveSpeed);


	scene.Add(butterfly);


	constexpr float beeMoveSpeed{ 120 };
	
	auto bee = std::make_shared<dae::GameObject>();
	bee->Init();
	bee->AddComponent<dae::ImageRenderComponent>();
	bee->GetTransform()->SetLocalPosition({ 350, 325, 0 });
	
	bee->GetComponent<dae::ImageComponent>()->SetTexture("Galaga_Bee.png");
	
	pCommand = input.AddKeyboardCommand<dae::MoveCommand>(std::make_unique<dae::MoveCommand>(bee.get()),
		dae::KeyboardInput{ SDL_SCANCODE_W, dae::ButtonState::Pressed });
	pCommand->SetDirection({ 0,1,0 });
	pCommand->SetMoveSpeed(-beeMoveSpeed);
	
	pCommand = input.AddKeyboardCommand<dae::MoveCommand>(std::make_unique<dae::MoveCommand>(bee.get()),
		dae::KeyboardInput{ SDL_SCANCODE_S, dae::ButtonState::Pressed });
	pCommand->SetDirection({ 0,1,0 });
	pCommand->SetMoveSpeed(beeMoveSpeed);
	
	pCommand = input.AddKeyboardCommand<dae::MoveCommand>(std::make_unique<dae::MoveCommand>(bee.get()),
		dae::KeyboardInput{ SDL_SCANCODE_A, dae::ButtonState::Pressed });
	pCommand->SetDirection({ 1,0,0 });
	pCommand->SetMoveSpeed(-beeMoveSpeed);
	
	pCommand = input.AddKeyboardCommand<dae::MoveCommand>(std::make_unique<dae::MoveCommand>(bee.get()),
		dae::KeyboardInput{ SDL_SCANCODE_D, dae::ButtonState::Pressed });
	pCommand->SetDirection({ 1,0,0 });
	pCommand->SetMoveSpeed(beeMoveSpeed);
	
	
	scene.Add(bee);


}

void load()
{
	//Rotators();

	Input();

}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}