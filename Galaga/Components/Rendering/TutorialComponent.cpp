#include "TutorialComponent.h"

dae::TutorialComponent::TutorialComponent(std::weak_ptr<GameObject> owner)
	:Component(owner)
{
}

void dae::TutorialComponent::RenderImGui()
{
	ImGui::Begin("Tutorial", nullptr);

	ImGui::Text("Controls: \n");
	ImGui::Text("Player 1: \n Controller:\n Joystick left/right -> move left/right\n Button X -> Shoot");
	ImGui::Text("Player 1: \n Keyboard:\n A/D -> move left/right\n F -> Shoot");

	ImGui::End();
}
