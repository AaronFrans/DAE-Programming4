#include <SDL.h>
#include <imgui_impl_sdl2.h>
#include "InputManager.h"

bool dae::InputManager::ProcessInput()
{
	for (auto& controller : m_Controllers)
	{
		controller->Update();
	}

	for (auto& command : m_ControllerCommands)
	{
		const unsigned& index = command.first.first;
		const XboxController::ControllerButton& button = command.first.second;
		if (m_Controllers[index]->IsPressed(button))
		{
			command.second->Execute();
		}
	}


	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {

		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {

		}
		// etc...
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}

void dae::InputManager::HandleConrollerInputs()
{

}


unsigned dae::InputManager::AddController()
{
	unsigned index = static_cast<unsigned int>(m_Controllers.size());
	m_Controllers.push_back(std::make_unique<XboxController>(index));
	return index;
}
