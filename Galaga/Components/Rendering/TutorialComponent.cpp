#include "TutorialComponent.h"

dae::TutorialComponent::TutorialComponent(GameObject* owner)
	:Component(owner)
{
}

void dae::TutorialComponent::RenderImGui()
{
	ImGui::Begin("Tutorial", nullptr);
	ImGui::Text("Menus: \n");
	ImGui::Text("	Controls: \n");
	ImGui::Text("		Player 1: \n		Controller:\n		 A -> confirm\n		 DPad left/right -> switch button\n");
	ImGui::Text("		Player 1: \n		Keyboard:\n		 Space -> confirm\n		 left/right -> switch button\n");
	ImGui::NewLine();
	ImGui::Text("Singleplayer: \n");
	ImGui::Text("	Controls: \n");
	ImGui::Text("		Player 1: \n		Controller:\n		 button left/right -> move left/right\n		 Button X -> Shoot \n");
	ImGui::Text("		Player 1: \n		Keyboard:\n		 left/right -> move left/right\n		 F -> Shoot \n");
	ImGui::NewLine();
	ImGui::Text("COOP: \n");
	ImGui::Text("	Controls: \n");
	ImGui::Text("		Player 1: \n		Controller:\n		 button left/right -> move left/right\n		 Button X -> Shoot \n");
	ImGui::Text("		Player 1: \n		Keyboard:\n		 left/right -> move left/right\n		 F -> Shoot \n");
	ImGui::Text("		Player 2: \n		Controller:\n		 button left/right -> move left/right\n		 Button X -> Shoot \n");
	ImGui::NewLine();
	ImGui::Text("Versus: \n");
	ImGui::Text("	Controls: \n");
	ImGui::Text("		Player 1: \n Controller:\n		 button left/right -> move left/right\n		 Button X -> Shoot \n");
	ImGui::Text("		Player 1: \n Keyboard:\n		 left/right -> move left/right\n		 F -> Shoot \n");
	ImGui::Text("		Player 1: \n Controller:\n		 Button A -> Dive\n		 Button B -> Tractor \n");

	ImGui::End();
}
