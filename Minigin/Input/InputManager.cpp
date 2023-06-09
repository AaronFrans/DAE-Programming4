#include <SDL.h>
#include <imgui_impl_sdl2.h>
#include <iostream>
#include "InputManager.h"
#include <Scene/SceneManager.h>
#include <Scene/Scene.h>

bool dae::InputManager::ProccesCommands()
{
	if (!ProcessInput()) return false;

	HandleConrollerInputs();

	HandleKeyboardInputs();


	return true;

}

bool dae::InputManager::ProcessInput()
{
	m_PreviousState = m_CurrentState;

	SDL_Event e;
	while (SDL_PollEvent(&e)) {

		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			const auto pressedButton = SDL_GetScancodeFromKey(e.key.keysym.sym);

			m_CurrentState[pressedButton] = true;

		}
		if (e.type == SDL_KEYUP) {
			const auto releasedButton = SDL_GetScancodeFromKey(e.key.keysym.sym);

			m_CurrentState[releasedButton] = false;
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

	const auto activeSceneName = SceneManager::GetInstance().GetActiveScene().GetName();
	for (auto& controllerCommand : m_ControllerCommands)
	{
		const auto scene = controllerCommand.first.sceneName;
		if (activeSceneName != scene)
			continue;

		const unsigned index = controllerCommand.first.controllerIndex;
		const ButtonState buttonState = controllerCommand.first.buttonsState;
		const XboxController::ControllerButton button = controllerCommand.first.controllerKey;
		const auto command = controllerCommand.second.get();


		switch (buttonState)
		{
		case ButtonState::Up:
			if (m_Controllers[index]->IsUp(button))
				command->Execute();
			break;
		case ButtonState::Down:
			if (m_Controllers[index]->IsDown(button))
				command->Execute();
			break;
		case ButtonState::Pressed:
			if (m_Controllers[index]->IsPressed(button))
				command->Execute();
			break;
		}

	}
}

void dae::InputManager::HandleKeyboardInputs()
{
	const auto activeSceneName = SceneManager::GetInstance().GetActiveScene().GetName();

	for (auto& keyboardCommand : m_KeyboardCommands)
	{
		const auto scene = keyboardCommand.first.sceneName;
		if (activeSceneName != scene)
			continue;
		const unsigned scancode = keyboardCommand.first.keyboardScancode;
		const ButtonState buttonState = keyboardCommand.first.buttonsState;
		const auto command = keyboardCommand.second.get();

		switch (buttonState)
		{
		case ButtonState::Up:
			if (m_PreviousState[scancode] && !m_CurrentState[scancode])
				command->Execute();
			break;
		case ButtonState::Down:
			if (!m_PreviousState[scancode] && m_CurrentState[scancode])
				command->Execute();
			break;
		case ButtonState::Pressed:
			if (m_PreviousState[scancode] && m_CurrentState[scancode])
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
