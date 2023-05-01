#include "TutorialComponent.h"

dae::TutorialComponent::TutorialComponent(std::weak_ptr<GameObject> owner)
	:Component(owner)
{
}

void dae::TutorialComponent::RenderImGui()
{
	ImGui::Begin("Tutorial", nullptr);

	ImGui::Text("Controls: \n");
	ImGui::Text("Player 1: \n Controller button A -> Lose health\n Controller button B -> gain points\n");
	ImGui::Text("Player 2: \n Keyboard button q -> Lose health\n Keyboard button e -> gain points");

	ImGui::End();
}
