#include <SDL.h>
#include <imgui_impl_sdl2.h>
#include "InputManager.h"

bool dae::InputManager::ProccesCommands()
{
	if (!ProcessInput()) return false;


	HandleConrollerInputs();

	HandleKeyboardInputs();

	m_UpKeys.assign(m_UpKeys.size(), false);
	m_DownKeys.assign(m_DownKeys.size(), false);

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
			m_UpKeys[SDL_GetScancodeFromKey(e.key.keysym.sym)] = true;
		}
		if (e.type == SDL_KEYUP) {

			m_PressedKeys[SDL_GetScancodeFromKey(e.key.keysym.sym)] = false;
			m_UpKeys[SDL_GetScancodeFromKey(e.key.keysym.sym)] = true;
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

	for (auto& controllerCommand : m_ControllerCommands)
	{
		const unsigned& index = controllerCommand.first.first;
		const XboxController::ControllerButton& button = controllerCommand.first.second;
		const auto& command = controllerCommand.second.get();

		switch (command->GetButtonsState())
		{
		case Command::ButtonState::Up:
			if (m_Controllers[index]->IsUp(button))
				command->Execute();
			break;
		case Command::ButtonState::Down:
			if (m_Controllers[index]->IsDown(button))
				command->Execute();
			break;
		case Command::ButtonState::Pressed:
			if (m_Controllers[index]->IsPressed(button))
				command->Execute();
			break;
		}

	}
}

void dae::InputManager::HandleKeyboardInputs()
{
	for (auto& keyboardCommand : m_KeyboardCommands)
	{
		const unsigned& keyCode = keyboardCommand.first;
		const auto& command = keyboardCommand.second.get();
		switch (command->GetButtonsState())
		{
		case Command::ButtonState::Up:
			if (m_UpKeys[keyCode])
				command->Execute();
			break;
		case Command::ButtonState::Down:
			if (m_DownKeys[keyCode])
				command->Execute();
			break;
		case Command::ButtonState::Pressed:
			if (m_PressedKeys[keyCode])
				command->Execute();
			break;
		}


	}
}


unsigned dae::InputManager::AddController()
{
	unsigned index = static_cast<unsigned int>(m_Controllers.size());
	m_Controllers.push_back(std::make_unique<XboxController>(index));
	return index;
}
