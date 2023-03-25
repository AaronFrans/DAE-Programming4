#include <SDL.h>
#include <imgui_impl_sdl2.h>
#include "InputManager.h"

bool dae::InputManager::ProccesCommands()
{
	if (!ProcessInput()) return false;


	HandleConrollerInputs();

	HandleKeyboardInputs();

	return true;

}
bool dae::InputManager::ProcessInput()
{

	SDL_Event e;
	while (SDL_PollEvent(&e)) {

		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
		
			m_PressedKeys[SDL_GetScancodeFromKey(e.key.keysym.sym)] = true;
		}
		if (e.type == SDL_KEYUP) {
		
			m_PressedKeys[SDL_GetScancodeFromKey(e.key.keysym.sym)] = false;
		}
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			
		}


		// etc...
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}

void dae::InputManager::HandleConrollerInputs()
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
			command.second->Execute();
	}
}

void dae::InputManager::HandleKeyboardInputs()
{
	for (auto& command : m_KeyboardCommands)
	{
		const unsigned& keyCode = command.first;

		if (m_PressedKeys[keyCode])
			command.second->Execute();

	}
}


unsigned dae::InputManager::AddController()
{
	unsigned index = static_cast<unsigned int>(m_Controllers.size());
	m_Controllers.push_back(std::make_unique<XboxController>(index));
	return index;
}
